/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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
import LineChart from './chart/charts/LineChart';
import { LineChartModel } from './chart/charts/LineChart';
import { XAxis, XAxisPosition } from './chart/components/XAxis';
import YAxis, { AxisDependency, YAxisLabelPosition } from './chart/components/YAxis';
import LineData from './chart/data/LineData';
import { LineDataSet, ColorStop, Mode } from './chart/data/LineDataSet';
import EntryOhos from './chart/data/EntryOhos';
import { JArrayList } from './chart/utils/JArrayList';
import ILineDataSet from './chart/interfaces/datasets/ILineDataSet';
import { TIndexInfo } from '../../entity/DatabaseEntity';


@Component
export struct Temperature {
  @State private gpData: TIndexInfo[] = []
  topAxis: XAxis = new XAxis(); //顶部X轴
  bottomAxis: XAxis = new XAxis(); //底部X轴
  atWidth: number = 350; //表的宽度
  atHeight: number = 300; //表的高度
  minOffset: number = 15; //X轴线偏移量
  leftAxis: YAxis = null;
  rightAxis: YAxis = null;
  lineData: LineData = null;
  topAxis1: XAxis = new XAxis(); //顶部X轴
  bottomAxis1: XAxis = new XAxis(); //底部X轴
  leftAxis1: YAxis = null;
  rightAxis1: YAxis = null;
  lineData1: LineData = null;
  topAxis2: XAxis = new XAxis(); //顶部X轴
  bottomAxis2: XAxis = new XAxis(); //底部X轴
  leftAxis2: YAxis = null;
  rightAxis2: YAxis = null;
  lineData2: LineData = null;
  topAxis3: XAxis = new XAxis(); //顶部X轴
  bottomAxis3: XAxis = new XAxis(); //底部X轴
  leftAxis3: YAxis = null;
  rightAxis3: YAxis = null;
  lineData3: LineData = null;
  topAxis4: XAxis = new XAxis(); //顶部X轴
  bottomAxis4: XAxis = new XAxis(); //底部X轴
  leftAxis4: YAxis = null;
  rightAxis4: YAxis = null;
  lineData4: LineData = null;
  topAxis5: XAxis = new XAxis(); //顶部X轴
  bottomAxis5: XAxis = new XAxis(); //底部X轴
  leftAxis5: YAxis = null;
  rightAxis5: YAxis = null;
  lineData5: LineData = null;
  topAxis6: XAxis = new XAxis(); //顶部X轴
  bottomAxis6: XAxis = new XAxis(); //底部X轴
  leftAxis6: YAxis = null;
  rightAxis6: YAxis = null;
  lineData6: LineData = null;


  lineChartModel : LineChartModel = new LineChartModel();
  lineChartModel1 : LineChartModel = new LineChartModel();
  lineChartModel2 : LineChartModel = new LineChartModel();
  lineChartModel3 : LineChartModel = new LineChartModel();
  lineChartModel4 : LineChartModel = new LineChartModel();
  lineChartModel5 : LineChartModel = new LineChartModel();
  lineChartModel6 : LineChartModel = new LineChartModel();

  aboutToAppear() {

    this.lineData = this.initCurveData();

    if (this.gpData.length < 10) {
      this.topAxis.setLabelCount(this.gpData.length, false);
    } else {
      this.topAxis.setLabelCount(6, false);
    }

    this.topAxis.setPosition(XAxisPosition.TOP);
    this.topAxis.setAxisMinimum(0);
    this.topAxis.setAxisMaximum(this.gpData.length - 1);

    if (this.gpData.length < 10) {
      this.bottomAxis.setLabelCount(this.gpData.length, false);
    } else {
      this.bottomAxis.setLabelCount(6, false);
    }
    this.bottomAxis.setPosition(XAxisPosition.BOTTOM);
    this.bottomAxis.setAxisMinimum(0);
    this.bottomAxis.setAxisMaximum(this.gpData.length - 1);

    this.leftAxis = new YAxis(AxisDependency.LEFT);
    this.leftAxis.setLabelCount(11, false);
    this.leftAxis.setPosition(YAxisLabelPosition.OUTSIDE_CHART);
    this.leftAxis.setSpaceTop(15);
    this.leftAxis.setAxisMinimum(this.lineData.getYMin() - 5);
    this.leftAxis.setAxisMaximum(this.lineData.getYMax() + 5);
    this.leftAxis.enableGridDashedLine(10, 10, 0)
    this.rightAxis = new YAxis(AxisDependency.RIGHT);
    this.rightAxis.setDrawGridLines(false);
    this.rightAxis.setLabelCount(11, false);
    this.rightAxis.setSpaceTop(15);
    this.rightAxis.setAxisMinimum(this.lineData.getYMin() - 5); // this replaces setStartAtZero(true)
    this.rightAxis.setAxisMaximum(this.lineData.getYMax() + 5);
    this.topAxis.setDrawAxisLine(false);
    this.bottomAxis.setDrawAxisLine(false);
    this.leftAxis.setAxisLineColor(Color.White)
    this.rightAxis.setAxisLineColor(Color.White)
    this.lineData1 = this.initCurveData1();

    if (this.gpData.length < 10) {
      this.topAxis1.setLabelCount(this.gpData.length, false);
    } else {
      this.topAxis1.setLabelCount(6, false);
    }

    this.topAxis1.setPosition(XAxisPosition.TOP);
    this.topAxis1.setAxisMinimum(0);
    this.topAxis1.setAxisMaximum(this.gpData.length - 1);

    if (this.gpData.length < 10) {
      this.bottomAxis1.setLabelCount(this.gpData.length, false);
    } else {
      this.bottomAxis1.setLabelCount(6, false);
    }
    this.bottomAxis1.setPosition(XAxisPosition.BOTTOM);
    this.bottomAxis1.setAxisMinimum(0);
    this.bottomAxis1.setAxisMaximum(this.gpData.length - 1);

    this.leftAxis1 = new YAxis(AxisDependency.LEFT);
    this.leftAxis1.setLabelCount(11, false);
    this.leftAxis1.setPosition(YAxisLabelPosition.OUTSIDE_CHART);
    this.leftAxis1.setSpaceTop(15);
    this.leftAxis1.setAxisMinimum(this.lineData1.getYMin() - 5);
    this.leftAxis1.setAxisMaximum(this.lineData1.getYMax() + 5);
    this.leftAxis1.enableGridDashedLine(10, 10, 0)
    this.rightAxis1 = new YAxis(AxisDependency.RIGHT);
    this.rightAxis1.setDrawGridLines(false);
    this.rightAxis1.setLabelCount(11, false);
    this.rightAxis1.setSpaceTop(15);
    this.rightAxis1.setAxisMinimum(this.lineData1.getYMin() - 5); // this replaces setStartAtZero(true)
    this.rightAxis1.setAxisMaximum(this.lineData1.getYMax() + 5);
    this.topAxis1.setDrawAxisLine(false);
    this.bottomAxis1.setDrawAxisLine(false);
    this.leftAxis1.setAxisLineColor(Color.White)
    this.rightAxis1.setAxisLineColor(Color.White)

    this.lineData2 = this.initCurveData2();

    if (this.gpData.length < 10) {
      this.topAxis2.setLabelCount(this.gpData.length, false);
    } else {
      this.topAxis2.setLabelCount(6, false);
    }

    this.topAxis2.setPosition(XAxisPosition.TOP);
    this.topAxis2.setAxisMinimum(0);
    this.topAxis2.setAxisMaximum(this.gpData.length - 1);

    if (this.gpData.length < 10) {
      this.bottomAxis2.setLabelCount(this.gpData.length, false);
    } else {
      this.bottomAxis2.setLabelCount(6, false);
    }
    this.bottomAxis2.setPosition(XAxisPosition.BOTTOM);
    this.bottomAxis2.setAxisMinimum(0);
    this.bottomAxis2.setAxisMaximum(this.gpData.length - 1);

    this.leftAxis2 = new YAxis(AxisDependency.LEFT);
    this.leftAxis2.setLabelCount(11, false);
    this.leftAxis2.setPosition(YAxisLabelPosition.OUTSIDE_CHART);
    this.leftAxis2.setSpaceTop(15);
    this.leftAxis2.setAxisMinimum(this.lineData2.getYMin() - 5);
    this.leftAxis2.setAxisMaximum(this.lineData2.getYMax() + 5);
    this.leftAxis2.enableGridDashedLine(10, 10, 0)
    this.rightAxis2 = new YAxis(AxisDependency.RIGHT);
    this.rightAxis2.setDrawGridLines(false);
    this.rightAxis2.setLabelCount(11, false);
    this.rightAxis2.setSpaceTop(15);
    this.rightAxis2.setAxisMinimum(this.lineData2.getYMin() - 5); // this replaces setStartAtZero(true)
    this.rightAxis2.setAxisMaximum(this.lineData2.getYMax() + 5);
    this.topAxis2.setDrawAxisLine(false);
    this.bottomAxis2.setDrawAxisLine(false);
    this.leftAxis2.setAxisLineColor(Color.White)
    this.rightAxis2.setAxisLineColor(Color.White)

    this.lineData3 = this.initCurveData3();

    if (this.gpData.length < 10) {
      this.topAxis3.setLabelCount(this.gpData.length, false);
    } else {
      this.topAxis3.setLabelCount(6, false);
    }

    this.topAxis3.setPosition(XAxisPosition.TOP);
    this.topAxis3.setAxisMinimum(0);
    this.topAxis3.setAxisMaximum(this.gpData.length - 1);

    if (this.gpData.length < 10) {
      this.bottomAxis3.setLabelCount(this.gpData.length, false);
    } else {
      this.bottomAxis3.setLabelCount(6, false);
    }
    this.bottomAxis3.setPosition(XAxisPosition.BOTTOM);
    this.bottomAxis3.setAxisMinimum(0);
    this.bottomAxis3.setAxisMaximum(this.gpData.length - 1);

    this.leftAxis3 = new YAxis(AxisDependency.LEFT);
    this.leftAxis3.setLabelCount(11, false);
    this.leftAxis3.setPosition(YAxisLabelPosition.OUTSIDE_CHART);
    this.leftAxis3.setSpaceTop(15);
    this.leftAxis3.setAxisMinimum(this.lineData3.getYMin() - 5);
    this.leftAxis3.setAxisMaximum(this.lineData3.getYMax() + 5);
    this.leftAxis3.enableGridDashedLine(10, 10, 0)
    this.rightAxis3 = new YAxis(AxisDependency.RIGHT);
    this.rightAxis3.setDrawGridLines(false);
    this.rightAxis3.setLabelCount(11, false);
    this.rightAxis3.setSpaceTop(15);
    this.rightAxis3.setAxisMinimum(this.lineData3.getYMin() - 5); // this replaces setStartAtZero(true)
    this.rightAxis3.setAxisMaximum(this.lineData3.getYMax() + 5);
    this.topAxis3.setDrawAxisLine(false);
    this.bottomAxis3.setDrawAxisLine(false);
    this.leftAxis3.setAxisLineColor(Color.White)
    this.rightAxis3.setAxisLineColor(Color.White)

    this.lineData4 = this.initCurveData4();

    if (this.gpData.length < 10) {
      this.topAxis4.setLabelCount(this.gpData.length, false);
    } else {
      this.topAxis4.setLabelCount(6, false);
    }

    this.topAxis4.setPosition(XAxisPosition.TOP);
    this.topAxis4.setAxisMinimum(0);
    this.topAxis4.setAxisMaximum(this.gpData.length - 1);

    if (this.gpData.length < 10) {
      this.bottomAxis4.setLabelCount(this.gpData.length, false);
    } else {
      this.bottomAxis4.setLabelCount(6, false);
    }
    this.bottomAxis4.setPosition(XAxisPosition.BOTTOM);
    this.bottomAxis4.setAxisMinimum(0);
    this.bottomAxis4.setAxisMaximum(this.gpData.length - 1);

    this.leftAxis4 = new YAxis(AxisDependency.LEFT);
    this.leftAxis4.setLabelCount(11, false);
    this.leftAxis4.setPosition(YAxisLabelPosition.OUTSIDE_CHART);
    this.leftAxis4.setSpaceTop(15);
    this.leftAxis4.setAxisMinimum(this.lineData4.getYMin() - 5);
    this.leftAxis4.setAxisMaximum(this.lineData4.getYMax() + 5);
    this.leftAxis4.enableGridDashedLine(10, 10, 0)
    this.rightAxis4 = new YAxis(AxisDependency.RIGHT);
    this.rightAxis4.setDrawGridLines(false);
    this.rightAxis4.setLabelCount(11, false);
    this.rightAxis4.setSpaceTop(15);
    this.rightAxis4.setAxisMinimum(this.lineData4.getYMin() - 5); // this replaces setStartAtZero(true)
    this.rightAxis4.setAxisMaximum(this.lineData4.getYMax() + 5);
    this.topAxis4.setDrawAxisLine(false);
    this.bottomAxis4.setDrawAxisLine(false);
    this.leftAxis4.setAxisLineColor(Color.White)
    this.rightAxis4.setAxisLineColor(Color.White)

    this.lineData5 = this.initCurveData5();

    if (this.gpData.length < 10) {
      this.topAxis5.setLabelCount(this.gpData.length, false);
    } else {
      this.topAxis5.setLabelCount(6, false);
    }

    this.topAxis5.setPosition(XAxisPosition.TOP);
    this.topAxis5.setAxisMinimum(0);
    this.topAxis5.setAxisMaximum(this.gpData.length - 1);

    if (this.gpData.length < 10) {
      this.bottomAxis5.setLabelCount(this.gpData.length, false);
    } else {
      this.bottomAxis5.setLabelCount(6, false);
    }
    this.bottomAxis5.setPosition(XAxisPosition.BOTTOM);
    this.bottomAxis5.setAxisMinimum(0);
    this.bottomAxis5.setAxisMaximum(this.gpData.length - 1);

    this.leftAxis5 = new YAxis(AxisDependency.LEFT);
    this.leftAxis5.setLabelCount(11, false);
    this.leftAxis5.setPosition(YAxisLabelPosition.OUTSIDE_CHART);
    this.leftAxis5.setSpaceTop(15);
    this.leftAxis5.setAxisMinimum(this.lineData5.getYMin() - 5);
    this.leftAxis5.setAxisMaximum(this.lineData5.getYMax() + 5);
    this.leftAxis5.enableGridDashedLine(10, 10, 0)
    this.rightAxis5 = new YAxis(AxisDependency.RIGHT);
    this.rightAxis5.setDrawGridLines(false);
    this.rightAxis5.setLabelCount(11, false);
    this.rightAxis5.setSpaceTop(15);
    this.rightAxis5.setAxisMinimum(this.lineData5.getYMin() - 5); // this replaces setStartAtZero(true)
    this.rightAxis5.setAxisMaximum(this.lineData5.getYMax() + 5);
    this.topAxis5.setDrawAxisLine(false);
    this.bottomAxis5.setDrawAxisLine(false);
    this.leftAxis5.setAxisLineColor(Color.White)
    this.rightAxis5.setAxisLineColor(Color.White)

    this.lineData6 = this.initCurveData6();

    if (this.gpData.length < 10) {
      this.topAxis6.setLabelCount(this.gpData.length, false);
    } else {
      this.topAxis6.setLabelCount(6, false);
    }

    this.topAxis6.setPosition(XAxisPosition.TOP);
    this.topAxis6.setAxisMinimum(0);
    this.topAxis6.setAxisMaximum(this.gpData.length - 1);

    if (this.gpData.length < 10) {
      this.bottomAxis6.setLabelCount(this.gpData.length, false);
    } else {
      this.bottomAxis6.setLabelCount(6, false);
    }
    this.bottomAxis6.setPosition(XAxisPosition.BOTTOM);
    this.bottomAxis6.setAxisMinimum(0);
    this.bottomAxis6.setAxisMaximum(this.gpData.length - 1);

    this.leftAxis6 = new YAxis(AxisDependency.LEFT);
    this.leftAxis6.setLabelCount(11, false);
    this.leftAxis6.setPosition(YAxisLabelPosition.OUTSIDE_CHART);
    this.leftAxis6.setSpaceTop(15);
    this.leftAxis6.setAxisMinimum(this.lineData6.getYMin() - 5);
    this.leftAxis6.setAxisMaximum(this.lineData6.getYMax() + 5);
    this.leftAxis6.enableGridDashedLine(10, 10, 0)
    this.rightAxis6 = new YAxis(AxisDependency.RIGHT);
    this.rightAxis6.setDrawGridLines(false);
    this.rightAxis6.setLabelCount(11, false);
    this.rightAxis6.setSpaceTop(15);
    this.rightAxis6.setAxisMinimum(this.lineData6.getYMin() - 5); // this replaces setStartAtZero(true)
    this.rightAxis6.setAxisMaximum(this.lineData6.getYMax() + 5);

    this.topAxis6.setDrawAxisLine(false);
    this.bottomAxis6.setDrawAxisLine(false);
    this.leftAxis6.setAxisLineColor(Color.White)
    this.rightAxis6.setAxisLineColor(Color.White)


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

    this.lineChartModel1.setTopAxis(this.topAxis1);
    this.lineChartModel1.setBottomAxis(this.bottomAxis1);
    this.lineChartModel1.setWidth(this.atWidth);
    this.lineChartModel1.setHeight(this.atHeight);
    this.lineChartModel1.setMinOffset(this.minOffset);
    this.lineChartModel1.setLeftAxis(this.leftAxis1);
    this.lineChartModel1.setRightAxis(this.rightAxis1);
    this.lineChartModel1.setLineData(this.lineData1);
    this.lineChartModel1.setIsShowLegend(false);
    this.lineChartModel1.init();

    this.lineChartModel2.setTopAxis(this.topAxis2);
    this.lineChartModel2.setBottomAxis(this.bottomAxis2);
    this.lineChartModel2.setWidth(this.atWidth);
    this.lineChartModel2.setHeight(this.atHeight);
    this.lineChartModel2.setMinOffset(this.minOffset);
    this.lineChartModel2.setLeftAxis(this.leftAxis2);
    this.lineChartModel2.setRightAxis(this.rightAxis2);
    this.lineChartModel2.setLineData(this.lineData2);
    this.lineChartModel2.setIsShowLegend(false);
    this.lineChartModel2.init();

    this.lineChartModel3.setTopAxis(this.topAxis3);
    this.lineChartModel3.setBottomAxis(this.bottomAxis3);
    this.lineChartModel3.setWidth(this.atWidth);
    this.lineChartModel3.setHeight(this.atHeight);
    this.lineChartModel3.setMinOffset(this.minOffset);
    this.lineChartModel3.setLeftAxis(this.leftAxis3);
    this.lineChartModel3.setRightAxis(this.rightAxis3);
    this.lineChartModel3.setLineData(this.lineData3);
    this.lineChartModel3.setIsShowLegend(false);
    this.lineChartModel3.init();

    this.lineChartModel4.setTopAxis(this.topAxis4);
    this.lineChartModel4.setBottomAxis(this.bottomAxis4);
    this.lineChartModel4.setWidth(this.atWidth);
    this.lineChartModel4.setHeight(this.atHeight);
    this.lineChartModel4.setMinOffset(this.minOffset);
    this.lineChartModel4.setLeftAxis(this.leftAxis4);
    this.lineChartModel4.setRightAxis(this.rightAxis4);
    this.lineChartModel4.setLineData(this.lineData4);
    this.lineChartModel4.setIsShowLegend(false);
    this.lineChartModel4.init();

    this.lineChartModel5.setTopAxis(this.topAxis5);
    this.lineChartModel5.setBottomAxis(this.bottomAxis5);
    this.lineChartModel5.setWidth(this.atWidth);
    this.lineChartModel5.setHeight(this.atHeight);
    this.lineChartModel5.setMinOffset(this.minOffset);
    this.lineChartModel5.setLeftAxis(this.leftAxis5);
    this.lineChartModel5.setRightAxis(this.rightAxis5);
    this.lineChartModel5.setLineData(this.lineData5);
    this.lineChartModel5.setIsShowLegend(false);
    this.lineChartModel5.init();

    this.lineChartModel6.setTopAxis(this.topAxis6);
    this.lineChartModel6.setBottomAxis(this.bottomAxis6);
    this.lineChartModel6.setWidth(this.atWidth);
    this.lineChartModel6.setHeight(this.atHeight);
    this.lineChartModel6.setMinOffset(this.minOffset);
    this.lineChartModel6.setLeftAxis(this.leftAxis6);
    this.lineChartModel6.setRightAxis(this.rightAxis6);
    this.lineChartModel6.setLineData(this.lineData6);
    this.lineChartModel6.setIsShowLegend(false);
    this.lineChartModel6.init();
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
      if (gpDataCur.systemHTemp == '') {
        continue
      }
      values.add(new EntryOhos(Number(index).valueOf(), Number(gpDataCur.systemHTemp).valueOf() / 1000));
    }


    let gradientFillColor = [];
    gradientFillColor.push([0x0C0099CC, 0.2])
    gradientFillColor.push([0x7F0099CC, 0.4])
    gradientFillColor.push([0x0099CC, 1.0])

    let dataSet = new JArrayList<ILineDataSet>();

    let set1 = new LineDataSet(values, 'SystemHTemp(℃)');
    set1.setDrawFilled(false);
    set1.setMode(Mode.CUBIC_BEZIER);
    set1.setGradientFillColor(gradientFillColor)
    set1.setColorByColor(Color.Green);
    set1.setLineWidth(3)
    set1.setDrawValues(false);
    set1.setDrawCircles(false);
    set1.setCircleColor(Color.Blue);
    set1.setCircleRadius(8);
    set1.setCircleHoleRadius(4)
    set1.setCircleHoleColor(Color.Green)
    set1.setDrawCircleHole(false)
    dataSet.add(set1);
    return new LineData(dataSet)
  }

  private initCurveData1(): LineData {

    let values = new JArrayList<EntryOhos>();
    for (let index: number = 0; index < this.gpData.length; index++) {
      const gpDataCur = this.gpData[index];
      if (gpDataCur.batteryTemp == '') {
        continue
      }
      values.add(new EntryOhos(Number(index).valueOf(), Number(gpDataCur.batteryTemp).valueOf() / 1000));
    }


    let gradientFillColor = [];
    gradientFillColor.push([0x0C0099CC, 0.2])
    gradientFillColor.push([0x7F0099CC, 0.4])
    gradientFillColor.push([0x0099CC, 1.0])

    let dataSet = new JArrayList<ILineDataSet>();

    let set1 = new LineDataSet(values, 'BatteryTemp(℃)');
    set1.setDrawFilled(false);
    set1.setMode(Mode.CUBIC_BEZIER);
    set1.setGradientFillColor(gradientFillColor)
    set1.setColorByColor(Color.Red);
    set1.setLineWidth(3)
    set1.setDrawCircles(false);
    set1.setCircleColor(Color.Blue);
    set1.setCircleRadius(8);
    set1.setDrawValues(false);
    set1.setCircleHoleRadius(4)
    set1.setCircleHoleColor(Color.Red)
    set1.setDrawCircleHole(false)
    dataSet.add(set1);
    return new LineData(dataSet)
  }

  private initCurveData2(): LineData {

    let values = new JArrayList<EntryOhos>();
    for (let index: number = 0; index < this.gpData.length; index++) {
      const gpDataCur = this.gpData[index];
      if (gpDataCur.shellFrontTemp == '') {
        continue
      }
      values.add(new EntryOhos(Number(index).valueOf(), Number(gpDataCur.shellFrontTemp).valueOf() / 1000));
    }


    let gradientFillColor = [];
    gradientFillColor.push([0x0C0099CC, 0.2])
    gradientFillColor.push([0x7F0099CC, 0.4])
    gradientFillColor.push([0x0099CC, 1.0])

    let dataSet = new JArrayList<ILineDataSet>();

    let set1 = new LineDataSet(values, 'ShellFrontTemp(℃)');
    set1.setDrawFilled(false);
    set1.setMode(Mode.CUBIC_BEZIER);
    set1.setGradientFillColor(gradientFillColor)
    set1.setColorByColor(Color.Blue);
    set1.setLineWidth(3)
    set1.setDrawValues(false);
    set1.setDrawCircles(false);
    set1.setCircleColor(Color.Blue);
    set1.setCircleRadius(8);
    set1.setCircleHoleRadius(4)
    set1.setCircleHoleColor(Color.Blue)
    set1.setDrawCircleHole(false)
    dataSet.add(set1);
    return new LineData(dataSet)
  }

  private initCurveData3(): LineData {

    let values = new JArrayList<EntryOhos>();
    for (let index: number = 0; index < this.gpData.length; index++) {
      const gpDataCur = this.gpData[index];
      if (gpDataCur.shellFrameTemp == '') {
        continue
      }
      values.add(new EntryOhos(Number(index).valueOf(), Number(gpDataCur.shellFrameTemp).valueOf() / 1000));
    }


    let gradientFillColor = [];
    gradientFillColor.push([0x0C0099CC, 0.2])
    gradientFillColor.push([0x7F0099CC, 0.4])
    gradientFillColor.push([0x0099CC, 1.0])

    let dataSet = new JArrayList<ILineDataSet>();

    let set1 = new LineDataSet(values, 'ShellFrameTemp(℃)');
    set1.setDrawFilled(false);
    set1.setMode(Mode.CUBIC_BEZIER);
    set1.setGradientFillColor(gradientFillColor)
    set1.setColorByColor(Color.Green);
    set1.setLineWidth(3)
    set1.setDrawValues(false);
    set1.setDrawCircles(false);
    set1.setCircleColor(Color.Blue);
    set1.setCircleRadius(8);
    set1.setCircleHoleRadius(4)
    set1.setCircleHoleColor(Color.Green)
    set1.setDrawCircleHole(false)
    dataSet.add(set1);
    return new LineData(dataSet)
  }

  private initCurveData4(): LineData {

    let values = new JArrayList<EntryOhos>();
    for (let index: number = 0; index < this.gpData.length; index++) {
      const gpDataCur = this.gpData[index];
      if (gpDataCur.socThermalTemp == '') {
        continue
      }
      values.add(new EntryOhos(Number(index).valueOf(), Number(gpDataCur.socThermalTemp).valueOf() / 1000));
    }


    let gradientFillColor = [];
    gradientFillColor.push([0x0C0099CC, 0.2])
    gradientFillColor.push([0x7F0099CC, 0.4])
    gradientFillColor.push([0x0099CC, 1.0])

    let dataSet = new JArrayList<ILineDataSet>();

    let set1 = new LineDataSet(values, 'SocThermalTemp(℃)');
    set1.setDrawFilled(false);
    set1.setMode(Mode.CUBIC_BEZIER);
    set1.setGradientFillColor(gradientFillColor)
    set1.setColorByColor(Color.Red);
    set1.setLineWidth(3)
    set1.setDrawValues(false);
    set1.setDrawCircles(false);
    set1.setCircleColor(Color.Blue);
    set1.setCircleRadius(8);
    set1.setCircleHoleRadius(4)
    set1.setCircleHoleColor(Color.Red)
    set1.setDrawCircleHole(false)
    dataSet.add(set1);
    return new LineData(dataSet)
  }

  private initCurveData5(): LineData {

    let values = new JArrayList<EntryOhos>();
    for (let index: number = 0; index < this.gpData.length; index++) {
      const gpDataCur = this.gpData[index];
      if (gpDataCur.shellBackTemp == '') {
        continue
      }
      values.add(new EntryOhos(Number(index).valueOf(), Number(gpDataCur.shellBackTemp).valueOf() / 1000));
    }


    let gradientFillColor = [];
    gradientFillColor.push([0x0C0099CC, 0.2])
    gradientFillColor.push([0x7F0099CC, 0.4])
    gradientFillColor.push([0x0099CC, 1.0])

    let dataSet = new JArrayList<ILineDataSet>();

    let set1 = new LineDataSet(values, 'ShellBackTemp(℃)');
    set1.setDrawFilled(false);
    set1.setMode(Mode.CUBIC_BEZIER);
    set1.setGradientFillColor(gradientFillColor)
    set1.setColorByColor(Color.Blue);
    set1.setLineWidth(3)
    set1.setDrawValues(false);
    set1.setDrawCircles(false);
    set1.setCircleColor(Color.Blue);
    set1.setCircleRadius(8);
    set1.setCircleHoleRadius(4)
    set1.setCircleHoleColor(Color.Blue)
    set1.setDrawCircleHole(false)
    dataSet.add(set1);
    return new LineData(dataSet)
  }

  private initCurveData6(): LineData {

    let values = new JArrayList<EntryOhos>();
    for (let index: number = 0; index < this.gpData.length; index++) {
      const gpDataCur = this.gpData[index];
      if (gpDataCur.gpuTemp == '') {
        continue
      }
      values.add(new EntryOhos(Number(index).valueOf(), Number(gpDataCur.gpuTemp).valueOf()));
    }


    let gradientFillColor = [];
    gradientFillColor.push([0x0C0099CC, 0.2])
    gradientFillColor.push([0x7F0099CC, 0.4])
    gradientFillColor.push([0x0099CC, 1.0])

    let dataSet = new JArrayList<ILineDataSet>();

    let set1 = new LineDataSet(values, 'GPUTemp(℃)');
    set1.setDrawFilled(false);
    set1.setMode(Mode.CUBIC_BEZIER);
    set1.setGradientFillColor(gradientFillColor)
    set1.setColorByColor(Color.Green);
    set1.setLineWidth(3)
    set1.setDrawCircles(false);
    set1.setCircleColor(Color.Blue);
    set1.setCircleRadius(8);
    set1.setDrawValues(false);
    set1.setCircleHoleRadius(4)
    set1.setCircleHoleColor(Color.Green)
    set1.setDrawCircleHole(false)
    dataSet.add(set1);
    return new LineData(dataSet)
  }


  build() {
    Stack({ alignContent: Alignment.TopStart }) {
      Scroll() {
        Column({ space: 20 }) {

          LineChart({lineChartModel: this.lineChartModel5})
          Text('ShellBackTemp(℃)') {
          }.fontWeight(FontWeight.Bold).fontColor(Color.Blue).fontSize('15fp').textAlign(TextAlign.Center)

        }.width('100%').alignItems(HorizontalAlign.Center)
      }.width('100%')
    }.width('100%').height('100%')
  }
}
