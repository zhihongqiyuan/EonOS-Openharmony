/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the 'License');
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an 'AS IS' BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
import LineChart from './chart/charts/LineChart'
import { LineChartModel } from './chart/charts/LineChart'
import { XAxis, XAxisPosition } from './chart/components/XAxis';
import YAxis, { AxisDependency, YAxisLabelPosition } from './chart/components/YAxis'
import LineData from './chart/data/LineData';
import { LineDataSet, ColorStop, Mode } from './chart/data/LineDataSet';
import EntryOhos from './chart/data/EntryOhos';
import { JArrayList } from './chart/utils/JArrayList';
import ILineDataSet from './chart/interfaces/datasets/ILineDataSet'
import { TIndexInfo } from '../../../common/entity/DatabaseEntity';
import HandleLostFrame from './utils/HandleLostFrame';
import CalculationUtils from '../../utils/CalculationUtils';
import { FpsLostFrame } from './data/DetailCommon';
import SPLogger from '../../utils/SPLogger'

const TAG: string = 'Performance'
/**
 * 性能页面
 */
@Component
export struct Performance {
  @State private gpData: TIndexInfo[] = []
  @State lostList: FpsLostFrame[] = []
  public lostLine: Array<number> = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22]
  public fpsValues: number[] = []
  public fpsJitterValues: string[] = []
  public topAxis: XAxis = new XAxis(); //顶部X轴
  public bottomAxis: XAxis = new XAxis(); //底部X轴
  public atWidth: number = 350; //表的宽度
  public atHeight: number = 300; //表的高度
  public minOffset: number = 15; //X轴线偏移量
  public leftAxis: YAxis = null;
  public rightAxis: YAxis = null;
  public lineData: LineData = null;
  public lineChartModel : LineChartModel = new LineChartModel();

  aboutToAppear() {
    SPLogger.DEBUG(TAG, 'this.gpData.length' + this.gpData.length);
    this.lineData = this.initCurveData();
    if (this.gpData.length < 10) {
      this.topAxis.setLabelCount(this.gpData.length, false);
    } else {
      this.topAxis.setLabelCount(6, false);
    }

    this.topAxis.setPosition(XAxisPosition.TOP);
    this.topAxis.setAxisMinimum(0);
    this.topAxis.setAxisMaximum(this.gpData.length - 1);
    this.topAxis.setDrawAxisLine(false)
    if (this.gpData.length < 10) {
      this.bottomAxis.setLabelCount(this.gpData.length, false);
    } else {
      this.bottomAxis.setLabelCount(6, false);
    }
    this.bottomAxis.setPosition(XAxisPosition.BOTTOM);
    this.bottomAxis.setAxisMinimum(0);
    this.bottomAxis.setAxisMaximum(this.gpData.length - 1);
    this.bottomAxis.setDrawAxisLine(false);

    this.leftAxis = new YAxis(AxisDependency.LEFT);
    this.leftAxis.setDrawAxisLine(true)
    this.leftAxis.setLabelCount(11, false);
    this.leftAxis.setSpaceTop(15);
    this.leftAxis.setAxisMinimum(-10);
    this.leftAxis.setAxisMaximum(this.lineData.getYMax() * 2);
    this.leftAxis.enableGridDashedLine(10, 10, 0)
    this.leftAxis.setAxisLineColor(Color.White)
    this.rightAxis = new YAxis(AxisDependency.RIGHT);
    this.rightAxis.setDrawGridLines(false);
    this.rightAxis.setLabelCount(7, false);
    this.rightAxis.setSpaceTop(11);
    this.rightAxis.setAxisMinimum(-10); // this replaces setStartAtZero(true)
    this.rightAxis.setAxisMaximum(this.lineData.getYMax() * 2);
    this.rightAxis.setAxisLineColor(Color.White)
    this.lineChartModel.setTopAxis(this.topAxis);
    this.lineChartModel.setBottomAxis(this.bottomAxis);
    this.lineChartModel.setWidth(this.atWidth);
    this.lineChartModel.setHeight(this.atHeight);
    this.lineChartModel.setMinOffset(this.minOffset);
    this.lineChartModel.setLeftAxis(this.leftAxis);

    this.lineChartModel.setRightAxis(this.rightAxis);
    this.lineChartModel.setLineData(this.lineData);
    this.lineChartModel.setIsShowLegend(false);
    this.lineChartModel.init();

  }

  /**
   * 处理丢帧
   */
  private handleLostFrame() {
    let handleLostFrame = new HandleLostFrame(CalculationUtils.calculateFPSNew(this.fpsValues))
    let lostFrameMap = new Map<number, number>()
    for (let jitter of this.fpsJitterValues) {
      let jankMap = handleLostFrame.getJankMap(jitter)
      for (let key of jankMap.keys()) {
        if (lostFrameMap.get(key) != null && lostFrameMap.get(key) !== undefined) {
          lostFrameMap.set(key, lostFrameMap.get(key) + jankMap.get(key))
        } else {
          lostFrameMap.set(key, jankMap.get(key))
        }
      }
    }
    let sumLostFrame: number = 0
    for (let key of lostFrameMap.keys()) {
      sumLostFrame += parseInt(lostFrameMap.get(key).toString())
    }
    SPLogger.DEBUG(TAG,
      ' sumLostFrame' + sumLostFrame);
    for (let key of lostFrameMap.keys()) {
      SPLogger.DEBUG(TAG, 'value start  key: ' + key);
      SPLogger.DEBUG(TAG, 'value start  : ' + lostFrameMap.get(key).toString() + 'total: ' + sumLostFrame);
      let fpsLostFrame = new FpsLostFrame((parseInt(key.toString()) - 1).toString(), lostFrameMap.get(key).toString(),
      this.getPercent(parseInt(lostFrameMap.get(key).toString()), sumLostFrame).toString(), $r('app.color.color_fff'))
      this.lostList.push(fpsLostFrame)
    }
    let list = this.lostList.sort((a, b) => parseInt(a.key) - parseInt(b.key))
    this.lostList.unshift(new FpsLostFrame('丢帧', '丢帧次数', '占比', $r('app.color.colorPrimary')))
    SPLogger.DEBUG(TAG, 'this.lostList' + this.lostList.length)
    SPLogger.DEBUG(TAG, 'this.lostList JSON' + JSON.stringify(this.lostList))
    SPLogger.DEBUG(TAG, 'this.list JSON' + JSON.stringify(list))
  }

  /**
   * 获取百分比
   */
  private getPercent(value: number, total: number): String {
    SPLogger.DEBUG(TAG, 'value end  : ' + value + 'total: ' + total);
    if (isNaN(value) || isNaN(total)) {
      return '0.00%';
    }
    let result: String = total <= 0 ? '0.00%' : (Math.round(value / total * 10000) / 100.00).toFixed(2) + '%';
    SPLogger.DEBUG(TAG, 'value value / total * 10000 : ' + value / total * 10000);
    SPLogger.DEBUG(TAG, 'value Math.round(value / total * 10000) : ' + Math.round(value / total * 10000));
    SPLogger.DEBUG(TAG, 'value Math.round(value / total * 10000) : ' + (Math.round(value / total * 10000) / 100.00));
    SPLogger.DEBUG(TAG, 'value result : ' + result);
    return result;
  }

  /**
   * 初始化数据
   * @param count  曲线图点的个数
   * @param range  y轴范围
   */
  private initCurveData(): LineData {

    let values = new JArrayList<EntryOhos>();
    for (let index: number = 0; index < this.gpData.length; index++) {
      const gpDataCur = this.gpData[index];

      if (gpDataCur.fps == '' || gpDataCur.fps === undefined) {
        continue
      }
      this.fpsValues.push(parseInt(gpDataCur.fps.toString()))
      SPLogger.DEBUG(TAG, 'gpDataCur.fpsJitters' + gpDataCur.fpsJitters);
      this.fpsJitterValues.push(gpDataCur.fpsJitters.toString().replace('\'', ''))
      values.add(new EntryOhos(Number(index).valueOf(), Number(gpDataCur.fps).valueOf()));
    }
    SPLogger.DEBUG(TAG, 'this.fpsJitterValues' + JSON.stringify(this.fpsJitterValues));

    //处理丢帧
    this.handleLostFrame();


    let gradientFillColor = [];
    gradientFillColor.push([0x0C0099CC, 0.2])
    gradientFillColor.push([0x7F0099CC, 0.4])
    gradientFillColor.push([0x0099CC, 1.0])

    let dataSet = new JArrayList<ILineDataSet>();

    let set1 = new LineDataSet(values, 'fps(HZ)');
    set1.setDrawFilled(false);
    set1.setMode(Mode.CUBIC_BEZIER);
    set1.setDrawValues(false);
    set1.setGradientFillColor(gradientFillColor)
    set1.setColorByColor(Color.Green);
    set1.setLineWidth(3)
    set1.setDrawCircles(false);
    set1.setCircleColor(Color.Blue);
    set1.setCircleRadius(8);
    set1.setCircleHoleRadius(4)
    set1.setCircleHoleColor(Color.Green)
    set1.setDrawCircleHole(false)
    dataSet.add(set1);
    return new LineData(dataSet)
  }

  build() {

    Scroll() {
      Column({ space: '3vp' }) {
        LineChart({lineChartModel: this.lineChartModel})
        Text('fps(HZ)') {
        }.fontWeight(FontWeight.Bold).fontColor(Color.Green).fontSize('15fp').textAlign(TextAlign.Center)
        Text('丢帧统计表\n(注：该表用于统计每一秒的丢帧数, 帧率基线取决于当前数据的最高帧率,\n其中丢帧的指的是丢几帧, 丢帧次数为整场数据的左侧丢帧值得发生次数)') {
        }.fontWeight(FontWeight.Bold).fontColor($r('app.color.color_333')).fontSize('12fp').textAlign(TextAlign.Center)

        List() {
          ForEach(this.lostList, (lostFrame) => {
            ListItem() {
              Flex({ justifyContent: FlexAlign.Center, alignItems: ItemAlign.Center }) {
                Text(lostFrame.key)
                  .fontSize('12fp')
                  .fontColor($r('app.color.color_333'))
                  .height('30vp')
                  .width('20%')
                  .border({ width: '1vp', color: $r('app.color.color_999') })
                  .backgroundColor(lostFrame.color)
                  .textAlign(TextAlign.Center)

                Text(lostFrame.value)
                  .fontSize('12fp')
                  .fontColor($r('app.color.color_333'))
                  .height('30vp')
                  .width('20%')
                  .border({ width: '1vp', color: $r('app.color.color_999') })
                  .backgroundColor(lostFrame.color)
                  .textAlign(TextAlign.Center)

                Text(lostFrame.percent)
                  .fontSize('12fp')
                  .fontColor($r('app.color.color_333'))
                  .height('30vp')
                  .width('20%')
                  .border({ width: '1vp', color: $r('app.color.color_999') })
                  .backgroundColor(lostFrame.color)
                  .textAlign(TextAlign.Center)
              }.width('100%')
            }
          }, lostFrame => lostFrame.key)
        }.width('100%').height('100%').margin({ bottom: '50%' })
      }.alignItems(HorizontalAlign.Center)
    }.scrollable(ScrollDirection.Vertical).scrollBar(BarState.Auto).height('100%')

  }
}
