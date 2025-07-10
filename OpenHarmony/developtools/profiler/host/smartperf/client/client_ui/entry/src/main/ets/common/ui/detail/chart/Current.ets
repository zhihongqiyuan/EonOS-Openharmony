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
import LineChart from './charts/LineChart'
import { LineChartModel } from './charts/LineChart'
import { XAxis, XAxisPosition } from './components/XAxis';
import YAxis, { AxisDependency, YAxisLabelPosition } from './components/YAxis'
import LineData from './data/LineData';
import { LineDataSet, ColorStop, Mode } from './data/LineDataSet';
import EntryOhos from './data/EntryOhos';
import { JArrayList } from './utils/JArrayList';
import ILineDataSet from './interfaces/datasets/ILineDataSet'
import { TIndexInfo } from '../../../entity/DatabaseEntity';

const TAG = 'Current'
@Entry
@Component
export struct Current {
  @State gpData: Array<TIndexInfo> = []
  public topAxis: XAxis = new XAxis(); //顶部X轴
  public bottomAxis: XAxis = new XAxis(); //底部X轴
  public atWidth: number = globalThis.screenWith > 800 ? 700 : 350; //表的宽度
  public atHeight: number = globalThis.screenWith > 800 ? 200 : 300; //表的高度
  public minOffset: number = 15; //X轴线偏移量
  public leftAxis: YAxis = null;
  public rightAxis: YAxis = null;
  public lineData: LineData = null;
  public lineChartModel : LineChartModel = new LineChartModel();

  aboutToAppear() {

    this.lineData = this.initCurveData();

    if (this.gpData.length < 10) {
      this.topAxis.setLabelCount(this.gpData.length, false);
    } else {
      this.topAxis.setLabelCount(6, false);
    }

    this.topAxis.setPosition(XAxisPosition.TOP);
    this.topAxis.setAxisMinimum(0);
    if (this.gpData.length == 2) {
      this.topAxis.setAxisMaximum(globalThis.testDuration);
    } else {
      this.topAxis.setAxisMaximum((this.gpData.length - 1) / 2);
    }

    if (this.gpData.length < 10) {
      this.bottomAxis.setLabelCount(this.gpData.length, false);
    } else {
      this.bottomAxis.setLabelCount(6, false);
    }
    this.bottomAxis.setPosition(XAxisPosition.BOTTOM);
    this.bottomAxis.setAxisMinimum(0);
    if (this.gpData.length == 2) {
      this.bottomAxis.setAxisMaximum(globalThis.testDuration);
    } else {
      this.bottomAxis.setAxisMaximum((this.gpData.length - 1) / 2);
    }

    this.leftAxis = new YAxis(AxisDependency.LEFT);
    this.leftAxis.setLabelCount(10, false);
    this.leftAxis.setPosition(YAxisLabelPosition.OUTSIDE_CHART);
    this.leftAxis.setSpaceTop(15);
    this.leftAxis.setAxisMinimum(this.lineData.getYMin() - 50); // this replaces setStartAtZero(true)
    this.leftAxis.setAxisMaximum(this.lineData.getYMax() + 50);
    this.leftAxis.enableGridDashedLine(10, 10, 0)

    this.rightAxis = new YAxis(AxisDependency.RIGHT);
    this.rightAxis.setDrawGridLines(false);
    this.rightAxis.setLabelCount(10, false);
    this.rightAxis.setSpaceTop(15);
    this.rightAxis.setAxisMinimum(this.lineData.getYMin() - 50); // this replaces setStartAtZero(true)
    this.rightAxis.setAxisMaximum(this.lineData.getYMax() + 50);

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
   * 初始化数据
   * @param count  曲线图点的个数
   * @param range  y轴范围
   */
  private initCurveData(): LineData {

    let values = new JArrayList<EntryOhos>();
    for (let index: number = 0; index < this.gpData.length; index++) {
      const gpDataCur = this.gpData[index];
      if (String(gpDataCur.currentNow) == '') {
        parseInt
        continue
      }

      if (this.gpData.length == 2) {
        values.add(new EntryOhos(Number(index).valueOf() * globalThis.testDuration,
        Math.round((Number(gpDataCur.currentNow).valueOf() * (Number(gpDataCur.voltageNow).valueOf() / 1e6)) / 3.8)
        ))
      } else {
        values.add(new EntryOhos(Number(index).valueOf() / 2,
        Math.round((Number(gpDataCur.currentNow).valueOf() * (Number(gpDataCur.voltageNow).valueOf() / 1e6)) / 3.8)
        ));
      }
    }


    let gradientFillColor: ColorStop[] = [];
    gradientFillColor.push([0x0C0099CC, 0.2])
    gradientFillColor.push([0x7F0099CC, 0.4])
    gradientFillColor.push([0x0099CC, 1.0])

    let dataSet = new JArrayList<ILineDataSet>();

    let set1 = new LineDataSet(values, 'Power Info(归一化电流 MA)');
    set1.setDrawFilled(false);
    set1.setMode(Mode.CUBIC_BEZIER);
    set1.setGradientFillColor(gradientFillColor)
    set1.setColorByColor(Color.Blue);
    set1.setLineWidth(2)
    set1.setDrawValues(false);
    set1.setDrawCircles(false);
    set1.setCircleColor(Color.Blue);
    set1.setCircleRadius(8);
    set1.setDrawValues(false);
    set1.setCircleHoleRadius(4)
    set1.setCircleHoleColor(Color.Blue)
    set1.setDrawCircleHole(false)
    dataSet.add(set1);
    return new LineData(dataSet)
  }

  build() {
    Stack({ alignContent: Alignment.TopStart }) {
      Scroll() {
        Column({ space: 20 }) {
          LineChart({lineChartModel: this.lineChartModel})
          Text('归一化电流 (mA)') {
          }.fontWeight(FontWeight.Bold).fontColor(Color.Blue).fontSize('15fp').textAlign(TextAlign.Center)
        }.width('100%').alignItems(HorizontalAlign.Center)
      }.width('100%')
    }.width('100%').height('100%')
  }
}
