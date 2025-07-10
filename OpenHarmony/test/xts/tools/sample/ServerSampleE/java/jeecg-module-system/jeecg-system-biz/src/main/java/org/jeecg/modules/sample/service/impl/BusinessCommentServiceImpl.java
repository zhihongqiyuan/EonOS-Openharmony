/*
 * Copyright (c) 2023 Hunan OpenValley Digital Industry Development Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.jeecg.modules.sample.service.impl;

import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import lombok.extern.slf4j.Slf4j;
import org.jeecg.modules.sample.entity.OhSampleBusinessComment;
import org.jeecg.modules.sample.mapper.BusinessCommentMapper;
import org.jeecg.modules.sample.service.IBusinessCommentService;
import org.springframework.stereotype.Service;

/**
 * 评论服务实现类
 *
 * @since:2023-06-10
 * @Version:V2.0
 */
@Service
@Slf4j
public class BusinessCommentServiceImpl extends ServiceImpl<BusinessCommentMapper,
        OhSampleBusinessComment> implements IBusinessCommentService {

}
