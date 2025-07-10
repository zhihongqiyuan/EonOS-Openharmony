// Copyright (c) 2023 Huawei Device Co., Ltd.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

use std::mem::MaybeUninit;
use std::process::Child as StdChild;
use std::sync::{Mutex, MutexGuard, Once};

use crate::signal::unix::signal_return_watch;
use crate::signal::SignalKind;
use crate::sync::watch;
use crate::sync::watch::Receiver;

pub(crate) struct GlobalZombieChild {
    child_signal: Mutex<Option<watch::Receiver<()>>>,
    vec: Mutex<Vec<StdChild>>,
}

impl GlobalZombieChild {
    pub(crate) fn get_instance() -> &'static GlobalZombieChild {
        static mut GLOBAL_ZOMBIE_CHILD: MaybeUninit<GlobalZombieChild> = MaybeUninit::uninit();
        static ONCE: Once = Once::new();

        unsafe {
            ONCE.call_once(|| {
                let global = GlobalZombieChild {
                    child_signal: Mutex::new(None),
                    vec: Mutex::new(Vec::new()),
                };
                GLOBAL_ZOMBIE_CHILD = MaybeUninit::new(global);
            });
            &*GLOBAL_ZOMBIE_CHILD.as_ptr()
        }
    }

    pub(crate) fn push(&self, child: StdChild) {
        self.vec.lock().unwrap().push(child)
    }

    pub(crate) fn release_zombie(&self) {
        if let Ok(mut guard) = self.child_signal.try_lock() {
            match &mut *guard {
                Some(child_signal) => self.release_zombie_when_some_receiver(child_signal),
                None => self.release_zombile_when_no_receiver(&mut guard),
            }
        }
    }

    fn release_zombie_when_some_receiver(&self, child_signal: &mut Receiver<()>) {
        if child_signal.try_notified().and_then(Result::ok).is_some() {
            reap_vec(self.vec.lock().unwrap())
        }
    }

    fn release_zombile_when_no_receiver(&self, guard: &mut MutexGuard<Option<Receiver<()>>>) {
        let vec = self.vec.lock().unwrap();
        if !vec.is_empty() {
            if let Ok(recv) = signal_return_watch(SignalKind::child()) {
                **guard = Some(recv);
                reap_vec(vec);
            }
        }
    }
}

fn reap_vec(mut vec: MutexGuard<'_, Vec<StdChild>>) {
    vec.retain_mut(|child| {
        match child.try_wait() {
            // `Ok(None)` means the child has not exited
            Ok(None) => true,
            _ => false,
        }
    });
    drop(vec)
}
