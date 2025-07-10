/*
 * Copyright (C) 2025 Huawei Device Co., Ltd.
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
import { describe, beforeAll, afterAll, it, expect } from '@ohos/hypium';
import unifiedDataChannel from '@ohos.data.unifiedDataChannel';

const TAG = "[udmfStageTest]"

export default function StageUdmfFileMangerTest() {
  describe('StageUdmfFileMangerTest', () => {
    beforeAll(() => {
      console.log("*************StageUdmfFileMangerTest Test Begin*************");
    });

    afterAll(() => {
      console.log("*************StageUdmfFileMangerTest Test end*************");
    });

    const createUnifiedData = (recordCount = 1) => {
      const file = new unifiedDataChannel.File();
      file.details = { name: 'test', type: 'txt' };
      file.uri = 'schema://com.samples.test/files/test.txt';
      const unifiedData = new unifiedDataChannel.UnifiedData(file);

      for (let i = 0; i < recordCount; i++) {
        const fileRecord = new unifiedDataChannel.File();
        fileRecord.details = { name: `test${i}`, type: 'txt' };
        fileRecord.uri = `schema://com.samples.test/files/test${i}.txt`;
        unifiedData.addRecord(fileRecord);
      }

      return unifiedData;
    };

    const testInsertData = (intention: unifiedDataChannel.Intention, testId: number, usePromise: boolean, done: Function) => {
      const options: unifiedDataChannel.Options = { intention };
      const unifiedData = createUnifiedData();

      const startTime = Date.now();
      if (usePromise) {
        unifiedDataChannel.insertData(options, unifiedData)
          .then(data => {
            const endTime = Date.now();
            console.log(`${intention} insertData promise time: ${endTime - startTime}ms`);
            console.info(`insertData succeeded, key = ${data}, testId = ${testId}`);
            expect().assertTrue();
            done();
          })
          .catch(() => {
            console.error(`Failed to insertData, testId = ${testId}`);
            expect().assertFail();
            done();
          });
      } else {
        unifiedDataChannel.insertData(options, unifiedData, (err, data) => {
          const endTime = Date.now();
          console.log(`${intention} insertData callback time: ${endTime - startTime}ms`);
          if (err === undefined) {
            console.info(`insertData succeeded, key = ${data}, testId = ${testId}`);
            expect().assertTrue();
            done();
          } else {
            console.error(`Failed to insertData, testId = ${testId}, code is ${err.code}, message is ${err.message}`);
            expect().assertFail();
            done();
          }
        });
      }
    };

    const testQueryData = (options: unifiedDataChannel.Options, testId: number, usePromise: boolean, done: Function) => {
      const startTime = Date.now();
      if (usePromise) {
        unifiedDataChannel.queryData(options)
          .then(data => {
            const endTime = Date.now();
            console.log(`queryData promise time: ${endTime - startTime}ms`);
            console.info(`queryData succeeded, size = ${data.length}, testId = ${testId}`);
            expect().assertTrue();
            done();
          })
          .catch(() => {
            console.error(`Failed to queryData, testId = ${testId}`);
            expect().assertFail();
            done();
          });
      } else {
        unifiedDataChannel.queryData(options, (err, data) => {
          const endTime = Date.now();
          console.log(`queryData callback time: ${endTime - startTime}ms`);
          if (err === undefined) {
            console.info(`queryData succeeded, size = ${data.length}, testId = ${testId}`);
            expect().assertTrue();
            done();
          } else {
            console.error(`Failed to queryData, testId = ${testId}, code is ${err.code}, message is ${err.message}`);
            expect().assertFail();
            done();
          }
        });
      }
    };

    const testDeleteData = (options: unifiedDataChannel.Options, testId: number, usePromise: boolean, done: Function) => {
      const startTime = Date.now();
      if (usePromise) {
        unifiedDataChannel.deleteData(options)
          .then(deletedData => {
            const endTime = Date.now();
            console.log(`deleteData promise time: ${endTime - startTime}ms`);
            console.info(`deleteData succeeded, testId = ${testId}, deleted ${deletedData.length} items`);
            expect(deletedData.length > 0).assertTrue();
            done();
          })
          .catch(() => {
            console.error(`Failed to deleteData, testId = ${testId}`);
            expect().assertFail();
            done();
          });
      } else {
        unifiedDataChannel.deleteData(options, (err, deletedData) => {
          const endTime = Date.now();
          console.log(`deleteData callback time: ${endTime - startTime}ms`);
          if (err === undefined) {
            console.info(`deleteData succeeded, testId = ${testId}, deleted ${deletedData.length} items`);
            expect(deletedData.length > 0).assertTrue();
            done();
          } else {
            console.error(`Failed to deleteData, testId = ${testId}, code is ${err.code}, message is ${err.message}`);
            expect().assertFail();
            done();
          }
        });
      }
    };

    let validKey: string = '';
    const insertDataAndGetKey = (intention: unifiedDataChannel.Intention, recordCount: number, done: Function) => {
      const options: unifiedDataChannel.Options = { intention };
      const unifiedData = createUnifiedData(recordCount);

      unifiedDataChannel.insertData(options, unifiedData)
        .then(data => {
          validKey = data;
          console.info(`insertData succeeded, key = ${data}`);
          done();
        })
        .catch(() => {
          console.error('Failed to insertData');
          done();
        });
    };

          /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_ValidKey_ValidIntention_Test_100
       * @tc.name      : queryData with valid key and valid intention
       * @tc.desc      : Test queryData with valid key and valid intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_ValidKey_ValidIntention_Test_100', 1, (done: Function) => {
        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.PICKER };
        const unifiedData = createUnifiedData(1000);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { key: key, intention: unifiedDataChannel.Intention.PICKER };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = { key: key, intention: unifiedDataChannel.Intention.PICKER };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                expect().assertFail();
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_ValidKey_InvalidIntention_Test_200
       * @tc.name      : queryData with valid key and invalid intention
       * @tc.desc      : Test queryData with valid key and invalid intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_ValidKey_InvalidIntention_Test_200', 1, (done: Function) => {
        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.PICKER };
        const unifiedData = createUnifiedData(1);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { key: key, intention: 'InvalidIntention' as unifiedDataChannel.Intention };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = { key: key, intention: unifiedDataChannel.Intention.PICKER };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                expect().assertFail();
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 200');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_ValidKey_NoIntention_Test_300
       * @tc.name      : queryData with valid key and no intention
       * @tc.desc      : Test queryData with valid key and no intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_ValidKey_NoIntention_Test_300', 1, (done: Function) => {
        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.PICKER };
        const unifiedData = createUnifiedData(1);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { key: key };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = { key: key, intention : unifiedDataChannel.Intention.PICKER };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                expect().assertFail();
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 300');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_InvalidKey_ValidIntention_Test_400
       * @tc.name      : queryData with invalid key and valid intention
       * @tc.desc      : Test queryData with invalid key and valid intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_InvalidKey_ValidIntention_Test_400', 1, (done: Function) => {

        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.PICKER };
        const unifiedData = createUnifiedData(1);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { key: 'invalidKey', intention: unifiedDataChannel.Intention.PICKER };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = { key: 'invalidKey', intention : unifiedDataChannel.Intention.PICKER };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                expect().assertFail();
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 400');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_InvalidKey_InvalidIntention_Test_500
       * @tc.name      : queryData with invalid key and invalid intention
       * @tc.desc      : Test queryData with invalid key and invalid intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_InvalidKey_InvalidIntention_Test_500', 1, (done: Function) => {
        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.PICKER };
        const unifiedData = createUnifiedData(1);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { key: 'invalidKey', intention: 'InvalidIntention' as unifiedDataChannel.Intention };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = { key: 'invalidKey', intention : unifiedDataChannel.Intention.PICKER };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                expect().assertFail();
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 500');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_InvalidKey_NoIntention_Test_600
       * @tc.name      : queryData with invalid key and no intention
       * @tc.desc      : Test queryData with invalid key and no intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_InvalidKey_NoIntention_Test_600', 1, (done: Function) => {

        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.PICKER };
        const unifiedData = createUnifiedData(1);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { key: 'invalidKey' };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = { key: 'invalidKey' , intention: unifiedDataChannel.Intention.PICKER };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                expect().assertFail();
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 600');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_NoKey_ValidIntention_Test_700
       * @tc.name      : queryData with no key and valid intention
       * @tc.desc      : Test queryData with no key and valid intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_NoKey_ValidIntention_Test_700', 1, (done: Function) => {

        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.PICKER };
        const unifiedData = createUnifiedData(1);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { intention: unifiedDataChannel.Intention.PICKER };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = {key:key , intention : unifiedDataChannel.Intention.PICKER };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                expect().assertFail();
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 700');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_NoKey_InvalidIntention_Test_800
       * @tc.name      : queryData with no key and invalid intention
       * @tc.desc      : Test queryData with no key and invalid intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_NoKey_InvalidIntention_Test_800', 1, (done: Function) => {

        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.PICKER };
        const unifiedData = createUnifiedData(1);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { intention: 'InvalidIntention' as unifiedDataChannel.Intention };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = {key:key, intention : unifiedDataChannel.Intention.PICKER };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                expect().assertFail();
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 800');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_NoKey_NoIntention_Test_900
       * @tc.name      : queryData with no key and no intention
       * @tc.desc      : Test queryData with no key and no intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_NoKey_NoIntention_Test_900', 1, (done: Function) => {

        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.PICKER };
        const unifiedData = createUnifiedData(1);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = {};
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = {key:key, intention : unifiedDataChannel.Intention.PICKER };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                expect().assertFail();
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 900');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_ValidKey_ValidIntention_Test_1000
       * @tc.name      : queryData with valid key and valid intention
       * @tc.desc      : Test queryData with valid key and valid intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_ValidKey_ValidIntention_Test_1000', 1, (done: Function) => {

        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.PICKER };
        const unifiedData = createUnifiedData(1);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { key:key, intention: unifiedDataChannel.Intention.PICKER };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = { key: key, intention: unifiedDataChannel.Intention.PICKER };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                console.error('Failed to query 1000');
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 1000');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_ValidKey_InvalidIntention_Test_1100
       * @tc.name      : queryData with valid key and invalid intention
       * @tc.desc      : Test queryData with valid key and invalid intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_ValidKey_InvalidIntention_Test_1100', 1, (done: Function) => {

        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.PICKER };
        const unifiedData = createUnifiedData(1);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { key:key, intention: 'InvalidIntention' as unifiedDataChannel.Intention };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = { key: key, intention: unifiedDataChannel.Intention.PICKER };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                console.error('Failed to query 1100');
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 1100');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_ValidKey_NoIntention_Test_1200
       * @tc.name      : queryData with valid key and no intention
       * @tc.desc      : Test queryData with valid key and no intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_ValidKey_NoIntention_Test_1200', 1, (done: Function) => {
        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.PICKER };
        const unifiedData = createUnifiedData(1);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { key:key };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = { key: key, intention: unifiedDataChannel.Intention.PICKER };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                console.error('Failed to query 1200');
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 1200');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_InvalidKey_ValidIntention_Test_1300
       * @tc.name      : queryData with invalid key and valid intention
       * @tc.desc      : Test queryData with invalid key and valid intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_InvalidKey_ValidIntention_Test_1300', 1, (done: Function) => {
        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.PICKER };
        const unifiedData = createUnifiedData(1);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { key: 'invalidKey', intention: unifiedDataChannel.Intention.PICKER };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                console.info('130000');
                const options: unifiedDataChannel.Options = { key: key, intention: unifiedDataChannel.Intention.PICKER };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                console.error('Failed to query 1300');
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 1300');
            done();
          });
      });


      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_NoKey_ValidIntention_Test_1600
       * @tc.name      : queryData with no key and valid intention
       * @tc.desc      : Test queryData with no key and valid intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_NoKey_ValidIntention_Test_1600', 1, (done: Function) => {
        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.DATA_HUB };
        const unifiedData = createUnifiedData(1000);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { intention: unifiedDataChannel.Intention.DATA_HUB };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length >= 1).assertTrue();
                const options: unifiedDataChannel.Options = { key: key, intention: unifiedDataChannel.Intention.DATA_HUB };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                console.error('Failed to query 1600 ');
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 1600');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_NoKey_InvalidIntention_Test_1700
       * @tc.name      : queryData with no key and invalid intention
       * @tc.desc      : Test queryData with no key and invalid intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_NoKey_InvalidIntention_Test_1700', 1, (done: Function) => {
        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.DATA_HUB };
        const unifiedData = createUnifiedData(1000);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { intention: 'InvalidIntention' as unifiedDataChannel.Intention };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = { key: key, intention: unifiedDataChannel.Intention.DATA_HUB };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                console.error('Failed to query 1700');
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 1700');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_ValidKey_ValidIntention_Test_1800
       * @tc.name      : queryData with valid key and valid intention
       * @tc.desc      : Test queryData with valid key and valid intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_ValidKey_ValidIntention_Test_1800', 1, (done: Function) => {

        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.SYSTEM_SHARE };
        const unifiedData = createUnifiedData(1000);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { key: validKey, intention: unifiedDataChannel.Intention.SYSTEM_SHARE };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = { key: key, intention: unifiedDataChannel.Intention.SYSTEM_SHARE };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                console.error('Failed to query 1800');
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 1800');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_ValidKey_InvalidIntention_Test_1900
       * @tc.name      : queryData with valid key and invalid intention
       * @tc.desc      : Test queryData with valid key and invalid intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_ValidKey_InvalidIntention_Test_1900', 1, (done: Function) => {

        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.SYSTEM_SHARE };
        const unifiedData = createUnifiedData(1000);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { key: key, intention: unifiedDataChannel.Intention.SYSTEM_SHARE };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.SYSTEM_SHARE };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                console.error('Failed to query 1900');
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 1900');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_ValidKey_NoIntention_Test_2000
       * @tc.name      : queryData with valid key and no intention
       * @tc.desc      : Test queryData with valid key and no intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_ValidKey_NoIntention_Test_2000', 1, (done: Function) => {

        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.SYSTEM_SHARE };
        const unifiedData = createUnifiedData(1);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { key: key, intention: unifiedDataChannel.Intention.SYSTEM_SHARE };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const queryOptions: unifiedDataChannel.Options = { key: key };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                console.error('Failed to query 2000');
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 2000');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_InvalidKey_ValidIntention_Test_2100
       * @tc.name      : queryData with invalid key and valid intention
       * @tc.desc      : Test queryData with invalid key and valid intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_InvalidKey_ValidIntention_Test_2100', 1, (done: Function) => {

        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.SYSTEM_SHARE };
        const unifiedData = createUnifiedData(1);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { key: 'invalidKey', intention: unifiedDataChannel.Intention.SYSTEM_SHARE };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.SYSTEM_SHARE };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                console.error('Failed to query 2100');
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 2100');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_NoKey_ValidIntention_Test_2200
       * @tc.name      : queryData with no key and valid intention
       * @tc.desc      : Test queryData with no key and valid intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_NoKey_ValidIntention_Test_2200', 1, (done: Function) => {

        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.SYSTEM_SHARE };
        const unifiedData = createUnifiedData(1);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { intention: unifiedDataChannel.Intention.SYSTEM_SHARE };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = { key: key, intention : unifiedDataChannel.Intention.SYSTEM_SHARE };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                console.error('Failed to query 2200');
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 2200');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_ValidKey_ValidIntention_Test_2300
       * @tc.name      : queryData with valid key and valid intention
       * @tc.desc      : Test queryData with valid key and valid intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_ValidKey_ValidIntention_Test_2300', 1, (done: Function) => {

        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.SYSTEM_SHARE };
        const unifiedData = createUnifiedData(1);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { key: key, intention: unifiedDataChannel.Intention.SYSTEM_SHARE };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = { key: key, intention : unifiedDataChannel.Intention.SYSTEM_SHARE };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                console.error('Failed to query 2300');
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 2300');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_ValidKey_InvalidIntention_Test_2400
       * @tc.name      : queryData with valid key and invalid intention
       * @tc.desc      : Test queryData with valid key and invalid intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_ValidKey_InvalidIntention_Test_2400', 1, (done: Function) => {

        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.SYSTEM_SHARE };
        const unifiedData = createUnifiedData(1);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { key: key, intention: 'InvalidIntention' as unifiedDataChannel.Intention };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = { key: key, intention : unifiedDataChannel.Intention.SYSTEM_SHARE };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                console.error('Failed to query 2400');
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 2400');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_ValidKey_NoIntention_Test_2500
       * @tc.name      : queryData with valid key and no intention
       * @tc.desc      : Test queryData with valid key and no intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_ValidKey_NoIntention_Test_2500', 1, (done: Function) => {
        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.SYSTEM_SHARE };
        const unifiedData = createUnifiedData(1);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { key: key };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = { key: key, intention : unifiedDataChannel.Intention.SYSTEM_SHARE };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                console.error('Failed to query 2500');
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 2500');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_InvalidKey_ValidIntention_Test_2600
       * @tc.name      : queryData with invalid key and valid intention
       * @tc.desc      : Test queryData with invalid key and valid intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_InvalidKey_ValidIntention_Test_2600', 1, (done: Function) => {

        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.SYSTEM_SHARE };
        const unifiedData = createUnifiedData(1);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { key: 'invalidKey', intention: unifiedDataChannel.Intention.SYSTEM_SHARE };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = { key: key, intention : unifiedDataChannel.Intention.SYSTEM_SHARE };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                console.error('Failed to query 2600');
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 2600');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_NoKey_ValidIntention_Test_2700
       * @tc.name      : queryData with no key and valid intention
       * @tc.desc      : Test queryData with no key and valid intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_NoKey_ValidIntention_Test_2700', 0, (done: Function) => {

        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.SYSTEM_SHARE };
        const unifiedData = createUnifiedData(1);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { intention: unifiedDataChannel.Intention.SYSTEM_SHARE };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = {intention : unifiedDataChannel.Intention.SYSTEM_SHARE };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                console.error('Failed to query 2700');
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 2700');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_ValidKey_ValidIntention_Test_2800
       * @tc.name      : queryData with valid key and valid intention
       * @tc.desc      : Test queryData with valid key and valid intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_ValidKey_ValidIntention_Test_2800', 1, (done: Function) => {

        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.MENU };
        const unifiedData = createUnifiedData(1000);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { key: key, intention: unifiedDataChannel.Intention.MENU };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = { key: key, intention: unifiedDataChannel.Intention.MENU };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                console.error('Failed to query 2800');
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 2800');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_ValidKey_InvalidIntention_Test_2900
       * @tc.name      : queryData with valid key and invalid intention
       * @tc.desc      : Test queryData with valid key and invalid intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_ValidKey_InvalidIntention_Test_2900', 1, (done: Function) => {

        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.MENU };
        const unifiedData = createUnifiedData(1);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { key: key, intention: 'InvalidIntention' as unifiedDataChannel.Intention };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = { key: key, intention: unifiedDataChannel.Intention.MENU };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                console.error('Failed to query 2900');
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 2900');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_ValidKey_NoIntention_Test_3000
       * @tc.name      : queryData with valid key and no intention
       * @tc.desc      : Test queryData with valid key and no intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_ValidKey_NoIntention_Test_3000', 1, (done: Function) => {

        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.MENU };
        const unifiedData = createUnifiedData(1);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { key: key };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = { key: key, intention: unifiedDataChannel.Intention.MENU };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                console.error('Failed to query 3000');
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 3000');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_InvalidKey_ValidIntention_Test_3100
       * @tc.name      : queryData with invalid key and valid intention
       * @tc.desc      : Test queryData with invalid key and valid intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_InvalidKey_ValidIntention_Test_3100', 1, (done: Function) => {

        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.MENU };
        const unifiedData = createUnifiedData(1);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { key: 'invalidKey', intention: unifiedDataChannel.Intention.MENU };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = { key: key, intention: unifiedDataChannel.Intention.MENU };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                console.error('Failed to query 3100');
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 3100');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_NoKey_ValidIntention_Test_3200
       * @tc.name      : queryData with no key and valid intention
       * @tc.desc      : Test queryData with no key and valid intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_NoKey_ValidIntention_Test_3200', 1, (done: Function) => {

        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.MENU };
        const unifiedData = createUnifiedData(1);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { intention: unifiedDataChannel.Intention.MENU };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = { key: key, intention: unifiedDataChannel.Intention.MENU };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                console.error('Failed to query 3200');
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 3200');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_ValidKey_ValidIntention_Test_3300
       * @tc.name      : queryData with valid key and valid intention
       * @tc.desc      : Test queryData with valid key and valid intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_ValidKey_ValidIntention_Test_3300', 1, (done: Function) => {

        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.MENU };
        const unifiedData = createUnifiedData(1);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { key: key, intention: unifiedDataChannel.Intention.MENU };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = { key: key, intention: unifiedDataChannel.Intention.MENU };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                console.error('Failed to query 3300');
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 3300');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_ValidKey_InvalidIntention_Test_3400
       * @tc.name      : queryData with valid key and invalid intention
       * @tc.desc      : Test queryData with valid key and invalid intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_ValidKey_InvalidIntention_Test_3400', 1, (done: Function) => {

        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.MENU };
        const unifiedData = createUnifiedData(1);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { key: key, intention: 'InvalidIntention' as unifiedDataChannel.Intention };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = { key: key, intention: unifiedDataChannel.Intention.MENU };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                console.error('Failed to query 3400');
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 3400');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_ValidKey_NoIntention_Test_3500
       * @tc.name      : queryData with valid key and no intention
       * @tc.desc      : Test queryData with valid key and no intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_ValidKey_NoIntention_Test_3500', 1, (done: Function) => {

        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.MENU };
        const unifiedData = createUnifiedData(1);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { key: key };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = { key: key, intention: unifiedDataChannel.Intention.MENU };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                console.error('Failed to query 3500');
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 3500');
            done();
          });
      });

      /**
       * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_InvalidKey_ValidIntention_Test_3600
       * @tc.name      : queryData with invalid key and valid intention
       * @tc.desc      : Test queryData with invalid key and valid intention.
       * @tc.size      : MEDIUM
       * @tc.type      : Function
       * @tc.level     : Level 3
       */
      it('SUB_DistributedData_UDMF_SDK_Query_InvalidKey_ValidIntention_Test_3600', 1, (done: Function) => {

        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.MENU };
        const unifiedData = createUnifiedData(1);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { key: 'invalidKey', intention: unifiedDataChannel.Intention.MENU };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = { key: key, intention: unifiedDataChannel.Intention.MENU };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                console.error('Failed to query 3600');
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 3600');
            done();
          });
      });

    /**
     * @tc.number    : SUB_DistributedData_UDMF_SDK_Query_NoKey_ValidIntention_Test_3700
     * @tc.name      : queryData with no key and valid intention
     * @tc.desc      : Test queryData with no key and valid intention.
     * @tc.size      : MEDIUM
     * @tc.type      : Function
     * @tc.level     : Level 3
     */
      it('SUB_DistributedData_UDMF_SDK_Query_NoKey_ValidIntention_Test_3700', 1, (done: Function) => {

        const options: unifiedDataChannel.Options = { intention : unifiedDataChannel.Intention.MENU };
        const unifiedData = createUnifiedData(1);

        unifiedDataChannel.insertData(options, unifiedData)
          .then(key => {
            const queryOptions: unifiedDataChannel.Options = { intention: unifiedDataChannel.Intention.MENU };
            unifiedDataChannel.queryData(queryOptions)
              .then(data => {
                expect(data.length == 1).assertTrue();
                const options: unifiedDataChannel.Options = { key: key, intention: unifiedDataChannel.Intention.MENU };
                unifiedDataChannel.deleteData(options);
                done();
              })
              .catch(() => {
                console.error('Failed to query 3700');
                done();
              });
          })
          .catch(() => {
            console.error('Failed to insertData 3700');
            done();
          });
      });
  });
}