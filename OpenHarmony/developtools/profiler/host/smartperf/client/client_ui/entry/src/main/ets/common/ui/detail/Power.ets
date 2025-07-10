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
import { TIndexInfo } from '../../entity/DatabaseEntity';
import { SummaryItem } from '../../entity/LocalConfigEntity'
import { getCpuCoreInfo, } from '../../utils/SystemUtils';


@Entry
@Component
export struct Power {
  @State private gpData: TIndexInfo[] = []
  public topAxis: XAxis = new XAxis(); //顶部X轴
  public bottomAxis: XAxis = new XAxis(); //底部X轴
  public atWidth: number = 350; //表的宽度
  public atHeight: number = 300; //表的高度
  public minOffset: number = 15; //X轴线偏移量
  public leftAxis: YAxis = null;
  public rightAxis: YAxis = null;
  public lineData: LineData = null;
  public lineChartModel : LineChartModel = new LineChartModel();
  public lineData2: LineData = null;
  public lineChartModel2 : LineChartModel = new LineChartModel();
  public testDuration:string = globalThis.testDuration
  @State private summaryItems: SummaryItem[] = []
  private controller: TabsController = new TabsController()
  aboutToAppear() {

    var normalCurrentNow: number = 0
    var maxCurrentNow: number = 0
    var avgBatteryTemp: number = 27
    var maxBatteryTemp: number = 27
    var curVoltage: number = 0




    for (var index = 0; index < this.gpData.length; index++) {
      console.log('globalThis.TIndexInfo--batteryTemp ' + this.gpData[index].batteryTemp);
      console.log('globalThis.TIndexInfo--length: ' + this.gpData.length);
      const gpDataCur = this.gpData[index];
      let currentNow = Number(gpDataCur.currentNow).valueOf()
      curVoltage = Number(gpDataCur.voltageNow).valueOf() / 1e6
      let batteryTemp = Number(gpDataCur.batteryTemp).valueOf() / 10;
      avgBatteryTemp += batteryTemp
        batteryTemp > maxBatteryTemp ? maxBatteryTemp = batteryTemp : null
      normalCurrentNow += Math.abs(currentNow) * Math.abs(curVoltage) / 3.8; //机测电压 原型机3.8  wgr4.0
        (0 - currentNow ) > maxCurrentNow ? maxCurrentNow = (0 - currentNow ) : null

    }

    this.summaryItems.push(
      new SummaryItem($r('app.media.icon_normalized_current'), '归一化电流', (normalCurrentNow / this.gpData.length).toFixed(0) + 'mA', '#fff8f8'),
      new SummaryItem($r('app.media.icon_normalized_current'), '最大电流', maxCurrentNow.toFixed(0) + 'mA', '#fff8f8'),
      new SummaryItem($r('app.media.icon_normalized_current'), '电压', (curVoltage * 1000 / (this.gpData.length + 1)).toFixed(0) + 'mV', '#fff8f8'),
      new SummaryItem($r('app.media.icon_max_temperature'), '平均温度', (avgBatteryTemp / (this.gpData.length + 1)).toFixed(0) + '℃', '#fff8f8'),
      new SummaryItem($r('app.media.icon_max_temperature'), '最大温度', (maxBatteryTemp).toFixed(0) + '℃', '#fff8f8'),
      new SummaryItem($r('app.media.icon_normalized_current'), '归一耗电', (normalCurrentNow * parseInt(this.testDuration) / 3600 / (this.gpData.length - 1)).toFixed(5) + 'mAH', '#fff8f8'),
    )

    this.lineData = this.initCurveData();

    if (this.gpData.length < 10) {
      this.topAxis.setLabelCount(this.gpData.length, false);
    } else {
      this.topAxis.setLabelCount(6, false);
    }
    this.topAxis.setPosition(XAxisPosition.TOP);
    this.topAxis.setAxisMinimum(0);
    this.topAxis.setAxisMaximum(this.gpData.length - 1);
    this.topAxis.setDrawAxisLine(false);
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
    this.leftAxis.setLabelCount(10, false);
    this.leftAxis.setPosition(YAxisLabelPosition.OUTSIDE_CHART);
    this.leftAxis.setSpaceTop(15);
    this.leftAxis.setAxisMinimum(this.lineData.getYMin() - 50); // this replaces setStartAtZero(true)
    this.leftAxis.setAxisMaximum(this.lineData.getYMax() + 50);
    this.leftAxis.enableGridDashedLine(10, 10, 0)
    this.leftAxis.setAxisLineColor(Color.White)

    this.rightAxis = new YAxis(AxisDependency.RIGHT);
    this.rightAxis.setDrawGridLines(false);
    this.rightAxis.setLabelCount(10, false);
    this.rightAxis.setSpaceTop(15);
    this.rightAxis.setAxisMinimum(this.lineData.getYMin() - 50); // this replaces setStartAtZero(true)
    this.rightAxis.setAxisMaximum(this.lineData.getYMax() + 50);
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




    this.lineData2 = this.initCurveData2();

    if (this.gpData.length < 10) {
      this.topAxis.setLabelCount(this.gpData.length, false);
    } else {
      this.topAxis.setLabelCount(6, false);
    }
    this.topAxis.setPosition(XAxisPosition.TOP);
    this.topAxis.setAxisMinimum(0);
    this.topAxis.setAxisMaximum(this.gpData.length - 1);
    this.topAxis.setDrawAxisLine(false);
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
    this.leftAxis.setLabelCount(10, false);
    this.leftAxis.setPosition(YAxisLabelPosition.OUTSIDE_CHART);
    this.leftAxis.setSpaceTop(15);
    this.leftAxis.setAxisMinimum(this.lineData2.getYMin() - 50); // this replaces setStartAtZero(true)
    this.leftAxis.setAxisMaximum(this.lineData2.getYMax() + 50);
    this.leftAxis.enableGridDashedLine(10, 10, 0)
    this.leftAxis.setAxisLineColor(Color.White)

    this.rightAxis = new YAxis(AxisDependency.RIGHT);
    this.rightAxis.setDrawGridLines(false);
    this.rightAxis.setLabelCount(10, false);
    this.rightAxis.setSpaceTop(15);
    this.rightAxis.setAxisMinimum(this.lineData2.getYMin() - 50); // this replaces setStartAtZero(true)
    this.rightAxis.setAxisMaximum(this.lineData2.getYMax() + 50);
    this.rightAxis.setAxisLineColor(Color.White)

    this.lineChartModel2.setTopAxis(this.topAxis);
    this.lineChartModel2.setBottomAxis(this.bottomAxis);
    this.lineChartModel2.setWidth(this.atWidth);
    this.lineChartModel2.setHeight(this.atHeight);
    this.lineChartModel2.setMinOffset(this.minOffset);
    this.lineChartModel2.setLeftAxis(this.leftAxis);
    this.lineChartModel2.setRightAxis(this.rightAxis);
    this.lineChartModel2.setLineData(this.lineData2);
    this.lineChartModel2.setIsShowLegend(false);
    this.lineChartModel2.init();
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
      if (gpDataCur.currentNow == '') {
        parseInt
        continue
      }
      values.add(new EntryOhos(Number(index).valueOf(),
      Math.round((Number(gpDataCur.voltageNow).valueOf() / 1e6))
      ));
    }


    let gradientFillColor = [];
    gradientFillColor.push([0x0C0099CC, 0.2])
    gradientFillColor.push([0x7F0099CC, 0.4])
    gradientFillColor.push([0x0099CC, 1.0])

    let dataSet = new JArrayList<ILineDataSet>();

    let set1 = new LineDataSet(values, 'Power Info(归一化电流 MA)');

    set1.setMode(Mode.CUBIC_BEZIER);
    set1.setGradientFillColor(gradientFillColor)
    set1.setColorByColor(Color.Blue);
    set1.setLineWidth(3)
    set1.setDrawValues(false);
    set1.setDrawCircles(false);
    set1.setCircleColor(Color.Blue);
    set1.setCircleRadius(8);
    set1.setDrawValues(false);
    set1.setCircleHoleRadius(4)
    set1.setCircleHoleColor(Color.Blue)
    set1.setDrawCircleHole(false)
    set1.setDrawFilled(false);



    dataSet.add(set1);
    return new LineData(dataSet)
  }
  private initCurveData2(): LineData {

    let values = new JArrayList<EntryOhos>();
    for (let index: number = 0; index < this.gpData.length; index++) {
      const gpDataCur = this.gpData[index];
      if (gpDataCur.currentNow == '') {
        parseInt
        continue
      }
      values.add(new EntryOhos(Number(index).valueOf(),
      Math.round((Number(gpDataCur.currentNow).valueOf() * (Number(gpDataCur.voltageNow).valueOf() / 1e6)) / 3.8)
      ));
    }


    let gradientFillColor = [];
    gradientFillColor.push([0x0C0099CC, 0.2])
    gradientFillColor.push([0x7F0099CC, 0.4])
    gradientFillColor.push([0x0099CC, 1.0])

    let dataSet = new JArrayList<ILineDataSet>();

    let set1 = new LineDataSet(values, 'Power Info(归一化电流 MA)');

    set1.setMode(Mode.CUBIC_BEZIER);
    set1.setGradientFillColor(gradientFillColor)
    set1.setColorByColor(Color.Blue);
    set1.setLineWidth(3)
    set1.setDrawValues(false);
    set1.setDrawCircles(false);
    set1.setCircleColor(Color.Blue);
    set1.setCircleRadius(8);
    set1.setDrawValues(false);
    set1.setCircleHoleRadius(4)
    set1.setCircleHoleColor(Color.Blue)
    set1.setDrawCircleHole(false)
    set1.setDrawFilled(false);



    dataSet.add(set1);
    return new LineData(dataSet)
  }
  build() {
    Column() {
      Tabs({ barPosition: BarPosition.Start, index: 0, controller: this.controller }) {
        TabContent() {
          Column() {
            Grid() {
              ForEach(this.summaryItems, item => {
                GridItem() {
                  Row({ space: '3vp' }) {
                    Image(item.icon).width('25vp').height('25vp')
                    Text(item.content).fontSize('12fp').textAlign(TextAlign.Start)
                    Text(item.value).fontSize('10fp').textAlign(TextAlign.Start)
                  }.alignItems(VerticalAlign.Center).width('100%')
                }
                .width('90%')
                .align(Alignment.Center)
                .backgroundColor(item.backColor)
                .border({ radius: '5vp', color: '#ffffff' }).shadow({radius : 5})
                .margin({top: '20vp'})
                .padding('10vp')
              }, item => item.content)
            }.margin({left: '15%', right: '15%' }).width('90%')
            .columnsTemplate('1fr 1fr')
          }.width('100%')
          .height('100%')
        }.tabBar('数据')
        TabContent() {
          Column() {
            LineChart({lineChartModel: this.lineChartModel})
            Text('Power Info(归一化电流 MA)') {
            }.fontWeight(FontWeight.Bold).fontColor(Color.Blue).fontSize('15fp').textAlign(TextAlign.Center)

          }.width('100%')
          .height('100%')
        }.tabBar('电流')
        TabContent() {
          Column() {
            LineChart({lineChartModel: this.lineChartModel2})
            Text('电压(MV)') {
            }.fontWeight(FontWeight.Bold).fontColor(Color.Blue).fontSize('15fp').textAlign(TextAlign.Center)

          }.width('100%')
          .height('100%')
        }.tabBar('电压')
      }
      .backgroundColor('#f5f5f5')
      .barWidth(360)
      .scrollable(true)
      .barHeight(60)
      .width('100%')
      .height('100%')

    }.width('100%').height('100%')
  }
}
