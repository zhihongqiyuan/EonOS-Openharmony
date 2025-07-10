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
import window from '@ohos.window';
import CheckEmptyUtils from '../../utils/CheckEmptyUtils'

/**
 * 负载页面
 */
@Component
export struct Load {
  @State private gpData: TIndexInfo[] = []
  public atWidth: number = 350; //表的宽度
  public atHeight: number = 300; //表的高度
  public minOffset: number = 15; //X轴线偏移量

  //cpuLoad0 cpuLoad1 cpuLoad2 cpuLoad3
  public topAxis: XAxis = new XAxis(); //顶部X轴
  public bottomAxis: XAxis = new XAxis(); //底部X轴
  public leftAxis: YAxis = null;
  public rightAxis: YAxis = null;
  public lineData: LineData = null;

  //CPUFre0 CPUFre1  CPUFre2  CPUFre3
  public topAxis1: XAxis = new XAxis(); //顶部X轴
  public bottomAxis1: XAxis = new XAxis(); //底部X轴
  public leftAxis1: YAxis = null;
  public rightAxis1: YAxis = null;
  public lineData1: LineData = null;

  //CPULoad4 CPULoad5 CPULoad6
  public topAxis2: XAxis = new XAxis(); //顶部X轴
  public bottomAxis2: XAxis = new XAxis(); //底部X轴
  public leftAxis2: YAxis = null;
  public rightAxis2: YAxis = null;
  public lineData2: LineData = null;

  //CPUFre4 CPUFre5  CPUFre6
  public topAxis3: XAxis = new XAxis(); //顶部X轴
  public bottomAxis3: XAxis = new XAxis(); //底部X轴
  public leftAxis3: YAxis = null;
  public rightAxis3: YAxis = null;
  public lineData3: LineData = null;

  //CPULoad7
  public topAxis4: XAxis = new XAxis(); //顶部X轴
  public bottomAxis4: XAxis = new XAxis(); //底部X轴
  public leftAxis4: YAxis = null;
  public rightAxis4: YAxis = null;
  public lineData4: LineData = null;

  //CPUFre7
  public topAxis5: XAxis = new XAxis(); //顶部X轴
  public bottomAxis5: XAxis = new XAxis(); //底部X轴
  public leftAxis5: YAxis = null;
  public rightAxis5: YAxis = null;
  public lineData5: LineData = null;

  //GPULoad
  public topAxis6: XAxis = new XAxis(); //顶部X轴
  public bottomAxis6: XAxis = new XAxis(); //底部X轴
  public leftAxis6: YAxis = null;
  public rightAxis6: YAxis = null;
  public lineData6: LineData = null;

  //GPUFre
  public topAxis7: XAxis = new XAxis(); //顶部X轴
  public bottomAxis7: XAxis = new XAxis(); //底部X轴
  public leftAxis7: YAxis = null;
  public rightAxis7: YAxis = null;
  public lineData7: LineData = null;

  //DDRFre
  public topAxis8: XAxis = new XAxis(); //顶部X轴
  public bottomAxis8: XAxis = new XAxis(); //底部X轴
  public leftAxis8: YAxis = null;
  public rightAxis8: YAxis = null;
  public lineData8: LineData = null;

  public lineChartModel : LineChartModel = new LineChartModel();
  public lineChartModel1 : LineChartModel = new LineChartModel();
  public lineChartModel2 : LineChartModel = new LineChartModel();
  public lineChartModel3 : LineChartModel = new LineChartModel();
  public lineChartModel4 : LineChartModel = new LineChartModel();
  public lineChartModel5 : LineChartModel = new LineChartModel();
  public lineChartModel6 : LineChartModel = new LineChartModel();
  public lineChartModel7 : LineChartModel = new LineChartModel();
  public lineChartModel8 : LineChartModel = new LineChartModel();

  aboutToAppear() {
    //CPULoad 0-3
    this.lineData = this.initCpuLoad0to3Data();

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
    this.leftAxis.setLabelCount(11, false);
    this.leftAxis.setPosition(YAxisLabelPosition.OUTSIDE_CHART);
    this.leftAxis.setSpaceTop(15);
    this.leftAxis.setAxisMinimum(this.lineData.getYMin() - 10);
    this.leftAxis.setAxisMaximum(this.lineData.getYMax() + 10);
    this.leftAxis.enableGridDashedLine(10, 10, 0)
    this.leftAxis.setAxisLineColor(Color.White)

    this.rightAxis = new YAxis(AxisDependency.RIGHT);
    this.rightAxis.setDrawGridLines(false);
    this.rightAxis.setLabelCount(11, false);
    this.rightAxis.setSpaceTop(15);
    this.rightAxis.setAxisMinimum(this.lineData.getYMin() - 10); // this replaces setStartAtZero(true)
    this.rightAxis.setAxisMaximum(this.lineData.getYMax() + 10);
    this.rightAxis.setAxisLineColor(Color.White)

    //CPUFre0
    this.lineData1 = this.initCpu0FreData();
    if (this.gpData.length < 10) {
      this.topAxis1.setLabelCount(this.gpData.length, false);
    } else {
      this.topAxis1.setLabelCount(6, false);
    }

    this.topAxis1.setPosition(XAxisPosition.TOP);
    this.topAxis1.setAxisMinimum(0);
    this.topAxis1.setAxisMaximum(this.gpData.length - 1);
    this.topAxis1.setDrawAxisLine(false);
    if (this.gpData.length < 10) {
      this.bottomAxis1.setLabelCount(this.gpData.length, false);
    } else {
      this.bottomAxis1.setLabelCount(6, false);
    }
    this.bottomAxis1.setPosition(XAxisPosition.BOTTOM);
    this.bottomAxis1.setAxisMinimum(0);
    this.bottomAxis1.setAxisMaximum(this.gpData.length - 1);
    this.bottomAxis1.setDrawAxisLine(false);
    this.leftAxis1 = new YAxis(AxisDependency.LEFT);
    this.leftAxis1.setLabelCount(11, false);
    this.leftAxis1.setPosition(YAxisLabelPosition.OUTSIDE_CHART);
    this.leftAxis1.setSpaceTop(15);
    this.leftAxis1.setAxisMinimum(this.lineData1.getYMin() - 500);
    this.leftAxis1.setAxisMaximum(this.lineData1.getYMax() + 500);
    this.leftAxis1.enableGridDashedLine(10, 10, 0)
    this.leftAxis1.setAxisLineColor(Color.White)
    this.rightAxis1 = new YAxis(AxisDependency.RIGHT);
    this.rightAxis1.setDrawGridLines(false);
    this.rightAxis1.setLabelCount(11, false);
    this.rightAxis1.setSpaceTop(15);
    this.rightAxis1.setAxisMinimum(this.lineData1.getYMin() - 500); // this replaces setStartAtZero(true)
    this.rightAxis1.setAxisMaximum(this.lineData1.getYMax() + 500);
    this.rightAxis1.setAxisLineColor(Color.White)
    //CPULoad4 CPULoad5 CPULoad6
    this.lineData2 = this.initCpuLoad4to6Data();
    if (this.gpData.length < 10) {
      this.topAxis2.setLabelCount(this.gpData.length, false);
    } else {
      this.topAxis2.setLabelCount(6, false);
    }

    this.topAxis2.setPosition(XAxisPosition.TOP);
    this.topAxis2.setAxisMinimum(0);
    this.topAxis2.setAxisMaximum(this.gpData.length - 1);
    this.topAxis2.setDrawAxisLine(false);

    if (this.gpData.length < 10) {
      this.bottomAxis2.setLabelCount(this.gpData.length, false);
    } else {
      this.bottomAxis2.setLabelCount(6, false);
    }
    this.bottomAxis2.setPosition(XAxisPosition.BOTTOM);
    this.bottomAxis2.setAxisMinimum(0);
    this.bottomAxis2.setAxisMaximum(this.gpData.length - 1);

    this.bottomAxis2.setDrawAxisLine(false);

    this.leftAxis2 = new YAxis(AxisDependency.LEFT);
    this.leftAxis2.setLabelCount(11, false);
    this.leftAxis2.setPosition(YAxisLabelPosition.OUTSIDE_CHART);
    this.leftAxis2.setSpaceTop(15);
    this.leftAxis2.setAxisMinimum(this.lineData2.getYMin() - 10);
    this.leftAxis2.setAxisMaximum(this.lineData2.getYMax() + 10);
    this.leftAxis2.enableGridDashedLine(10, 10, 0)

    this.leftAxis2.setAxisLineColor(Color.White)

    this.rightAxis2 = new YAxis(AxisDependency.RIGHT);
    this.rightAxis2.setDrawGridLines(false);
    this.rightAxis2.setLabelCount(11, false);
    this.rightAxis2.setSpaceTop(15);
    this.rightAxis2.setAxisMinimum(this.lineData2.getYMin() - 10); // this replaces setStartAtZero(true)
    this.rightAxis2.setAxisMaximum(this.lineData2.getYMax() + 10);

    this.rightAxis2.setAxisLineColor(Color.White)
    //CPUFre4
    this.lineData3 = this.initCpuFre4Data();
    if (this.gpData.length < 10) {
      this.topAxis3.setLabelCount(this.gpData.length, false);
    } else {
      this.topAxis3.setLabelCount(6, false);
    }

    this.topAxis3.setPosition(XAxisPosition.TOP);
    this.topAxis3.setAxisMinimum(0);
    this.topAxis3.setAxisMaximum(this.gpData.length - 1);
    this.topAxis3.setDrawAxisLine(false);

    if (this.gpData.length < 10) {
      this.bottomAxis3.setLabelCount(this.gpData.length, false);
    } else {
      this.bottomAxis3.setLabelCount(6, false);
    }
    this.bottomAxis3.setPosition(XAxisPosition.BOTTOM);
    this.bottomAxis3.setAxisMinimum(0);
    this.bottomAxis3.setAxisMaximum(this.gpData.length - 1);

    this.bottomAxis3.setDrawAxisLine(false);

    this.leftAxis3 = new YAxis(AxisDependency.LEFT);
    this.leftAxis3.setLabelCount(11, false);
    this.leftAxis3.setPosition(YAxisLabelPosition.OUTSIDE_CHART);
    this.leftAxis3.setSpaceTop(15);
    this.leftAxis3.setAxisMinimum(this.lineData3.getYMin() - 500);
    this.leftAxis3.setAxisMaximum(this.lineData3.getYMax() + 500);
    this.leftAxis3.enableGridDashedLine(10, 10, 0)

    this.leftAxis3.setAxisLineColor(Color.White)

    this.rightAxis3 = new YAxis(AxisDependency.RIGHT);
    this.rightAxis3.setDrawGridLines(false);
    this.rightAxis3.setLabelCount(11, false);
    this.rightAxis3.setSpaceTop(15);
    this.rightAxis3.setAxisMinimum(this.lineData3.getYMin() - 500); // this replaces setStartAtZero(true)
    this.rightAxis3.setAxisMaximum(this.lineData3.getYMax() + 500);

    this.rightAxis3.setAxisLineColor(Color.White)
    //CPULoad7
    this.lineData4 = this.initCpuLoad7Data();
    if (this.gpData.length < 10) {
      this.topAxis4.setLabelCount(this.gpData.length, false);
    } else {
      this.topAxis4.setLabelCount(6, false);
    }

    this.topAxis4.setPosition(XAxisPosition.TOP);
    this.topAxis4.setAxisMinimum(0);
    this.topAxis4.setAxisMaximum(this.gpData.length - 1);
    this.topAxis4.setDrawAxisLine(false);

    if (this.gpData.length < 10) {
      this.bottomAxis4.setLabelCount(this.gpData.length, false);
    } else {
      this.bottomAxis4.setLabelCount(6, false);
    }
    this.bottomAxis4.setPosition(XAxisPosition.BOTTOM);
    this.bottomAxis4.setAxisMinimum(0);
    this.bottomAxis4.setAxisMaximum(this.gpData.length - 1);

    this.bottomAxis4.setDrawAxisLine(false);

    this.leftAxis4 = new YAxis(AxisDependency.LEFT);
    this.leftAxis4.setLabelCount(8, false);
    this.leftAxis4.setPosition(YAxisLabelPosition.OUTSIDE_CHART);
    this.leftAxis4.setSpaceTop(15);
    this.leftAxis4.setAxisMinimum(this.lineData2.getYMin() - 10);
    this.leftAxis4.setAxisMaximum(this.lineData4.getYMax() + 10);
    this.leftAxis4.enableGridDashedLine(10, 10, 0)


    this.leftAxis4.setAxisLineColor(Color.White)


    this.rightAxis4 = new YAxis(AxisDependency.RIGHT);
    this.rightAxis4.setDrawGridLines(false);
    this.rightAxis4.setLabelCount(8, false);
    this.rightAxis4.setSpaceTop(15);
    this.rightAxis4.setAxisMinimum(this.lineData2.getYMin() - 10); // this replaces setStartAtZero(true)
    this.rightAxis4.setAxisMaximum(this.lineData4.getYMax() + 10);

    this.rightAxis4.setAxisLineColor(Color.White)
    //CPUFre7
    this.lineData5 = this.initCpuFre7Data();
    if (this.gpData.length < 10) {
      this.topAxis5.setLabelCount(this.gpData.length, false);
    } else {
      this.topAxis5.setLabelCount(6, false);
    }

    this.topAxis5.setPosition(XAxisPosition.TOP);
    this.topAxis5.setAxisMinimum(0);
    this.topAxis5.setAxisMaximum(this.gpData.length - 1);
    this.topAxis5.setDrawAxisLine(false);

    if (this.gpData.length < 10) {
      this.bottomAxis5.setLabelCount(this.gpData.length, false);
    } else {
      this.bottomAxis5.setLabelCount(6, false);
    }
    this.bottomAxis5.setPosition(XAxisPosition.BOTTOM);
    this.bottomAxis5.setAxisMinimum(0);
    this.bottomAxis5.setAxisMaximum(this.gpData.length - 1);

    this.bottomAxis5.setDrawAxisLine(false);

    this.leftAxis5 = new YAxis(AxisDependency.LEFT);
    this.leftAxis5.setLabelCount(8, false);
    this.leftAxis5.setPosition(YAxisLabelPosition.OUTSIDE_CHART);
    this.leftAxis5.setSpaceTop(15);
    this.leftAxis5.setAxisMinimum(this.lineData5.getYMin() - 500);
    this.leftAxis5.setAxisMaximum(this.lineData5.getYMax() + 500);
    this.leftAxis5.enableGridDashedLine(10, 10, 0)

    this.leftAxis5.setAxisLineColor(Color.White)

    this.rightAxis5 = new YAxis(AxisDependency.RIGHT);
    this.rightAxis5.setDrawGridLines(false);
    this.rightAxis5.setLabelCount(8, false);
    this.rightAxis5.setSpaceTop(15);
    this.rightAxis5.setAxisMinimum(this.lineData5.getYMin() - 500); // this replaces setStartAtZero(true)
    this.rightAxis5.setAxisMaximum(this.lineData5.getYMax() + 500);
    this.rightAxis5.setAxisLineColor(Color.White)
    //GPULoad
    this.lineData6 = this.initGpuLoadData();
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
    this.leftAxis6.setLabelCount(8, false);
    this.leftAxis6.setPosition(YAxisLabelPosition.OUTSIDE_CHART);
    this.leftAxis6.setSpaceTop(15);
    this.leftAxis6.setAxisMinimum(this.lineData6.getYMin() - 10);
    this.leftAxis6.setAxisMaximum(this.lineData6.getYMax() + 10);
    this.leftAxis6.enableGridDashedLine(10, 10, 0)
    this.rightAxis6 = new YAxis(AxisDependency.RIGHT);
    this.rightAxis6.setDrawGridLines(false);
    this.rightAxis6.setLabelCount(8, false);
    this.rightAxis6.setSpaceTop(15);
    this.rightAxis6.setAxisMinimum(this.lineData6.getYMin() - 10); // this replaces setStartAtZero(true)
    this.rightAxis6.setAxisMaximum(this.lineData6.getYMax() + 10);
    this.topAxis6.setDrawAxisLine(false);
    this.bottomAxis6.setDrawAxisLine(false);
    this.leftAxis6.setAxisLineColor(Color.White)
    this.rightAxis6.setAxisLineColor(Color.White)
    //GPUFre
    this.lineData7 = this.initGPUFreData();
    if (this.gpData.length < 10) {
      this.topAxis7.setLabelCount(this.gpData.length, false);
    } else {
      this.topAxis7.setLabelCount(6, false);
    }

    this.topAxis7.setPosition(XAxisPosition.TOP);
    this.topAxis7.setAxisMinimum(0);
    this.topAxis7.setAxisMaximum(this.gpData.length - 1);

    if (this.gpData.length < 10) {
      this.bottomAxis7.setLabelCount(this.gpData.length, false);
    } else {
      this.bottomAxis7.setLabelCount(6, false);
    }
    this.bottomAxis7.setPosition(XAxisPosition.BOTTOM);
    this.bottomAxis7.setAxisMinimum(0);
    this.bottomAxis7.setAxisMaximum(this.gpData.length - 1);

    this.leftAxis7 = new YAxis(AxisDependency.LEFT);
    this.leftAxis7.setLabelCount(8, false);
    this.leftAxis7.setPosition(YAxisLabelPosition.OUTSIDE_CHART);
    this.leftAxis7.setSpaceTop(15);
    this.leftAxis7.setAxisMinimum(this.lineData7.getYMin() - 500);
    this.leftAxis7.setAxisMaximum(this.lineData7.getYMax() + 500);
    this.leftAxis7.enableGridDashedLine(10, 10, 0)
    this.rightAxis7 = new YAxis(AxisDependency.RIGHT);
    this.rightAxis7.setDrawGridLines(false);
    this.rightAxis7.setLabelCount(8, false);
    this.rightAxis7.setSpaceTop(15);
    this.rightAxis7.setAxisMinimum(this.lineData7.getYMin() - 500); // this replaces setStartAtZero(true)
    this.rightAxis7.setAxisMaximum(this.lineData7.getYMax() + 500);
    this.topAxis7.setDrawAxisLine(false);
    this.bottomAxis7.setDrawAxisLine(false);
    this.leftAxis7.setAxisLineColor(Color.White)
    this.rightAxis7.setAxisLineColor(Color.White)
    //DDRFre
    this.lineData8 = this.initDDRFreData();
    if (this.gpData.length < 10) {
      this.topAxis8.setLabelCount(this.gpData.length, false);
    } else {
      this.topAxis8.setLabelCount(6, false);
    }

    this.topAxis8.setPosition(XAxisPosition.TOP);
    this.topAxis8.setAxisMinimum(0);
    this.topAxis8.setAxisMaximum(this.gpData.length - 1);

    if (this.gpData.length < 10) {
      this.bottomAxis8.setLabelCount(this.gpData.length, false);
    } else {
      this.bottomAxis8.setLabelCount(6, false);
    }
    this.bottomAxis8.setPosition(XAxisPosition.BOTTOM);
    this.bottomAxis8.setAxisMinimum(0);
    this.bottomAxis8.setAxisMaximum(this.gpData.length - 1);

    this.leftAxis8 = new YAxis(AxisDependency.LEFT);
    this.leftAxis8.setLabelCount(8, false);
    this.leftAxis8.setPosition(YAxisLabelPosition.OUTSIDE_CHART);
    this.leftAxis8.setSpaceTop(15);
    this.leftAxis8.setAxisMinimum(this.lineData8.getYMin() - 500);
    this.leftAxis8.setAxisMaximum(this.lineData8.getYMax() + 500);
    this.leftAxis8.enableGridDashedLine(10, 10, 0)
    this.rightAxis8 = new YAxis(AxisDependency.RIGHT);
    this.rightAxis8.setDrawGridLines(false);
    this.rightAxis8.setLabelCount(8, false);
    this.rightAxis8.setSpaceTop(15);
    this.rightAxis8.setAxisMinimum(this.lineData8.getYMin() - 500); // this replaces setStartAtZero(true)
    this.rightAxis8.setAxisMaximum(this.lineData8.getYMax() + 500);
    this.topAxis8.setDrawAxisLine(false);
    this.bottomAxis8.setDrawAxisLine(false);
    this.leftAxis8.setAxisLineColor(Color.White)
    this.rightAxis8.setAxisLineColor(Color.White)
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

    this.lineChartModel7.setTopAxis(this.topAxis7);
    this.lineChartModel7.setBottomAxis(this.bottomAxis7);
    this.lineChartModel7.setWidth(this.atWidth);
    this.lineChartModel7.setHeight(this.atHeight);
    this.lineChartModel7.setMinOffset(this.minOffset);
    this.lineChartModel7.setLeftAxis(this.leftAxis7);
    this.lineChartModel7.setRightAxis(this.rightAxis7);
    this.lineChartModel7.setLineData(this.lineData7);
    this.lineChartModel7.setIsShowLegend(false);
    this.lineChartModel7.init();

    this.lineChartModel8.setTopAxis(this.topAxis8);
    this.lineChartModel8.setBottomAxis(this.bottomAxis8);
    this.lineChartModel8.setWidth(this.atWidth);
    this.lineChartModel8.setHeight(this.atHeight);
    this.lineChartModel8.setMinOffset(this.minOffset);
    this.lineChartModel8.setLeftAxis(this.leftAxis8);
    this.lineChartModel8.setRightAxis(this.rightAxis8);
    this.lineChartModel8.setLineData(this.lineData8);
    this.lineChartModel8.setIsShowLegend(false);
    this.lineChartModel8.init();

  }

  /**
   * 初始化数据 cpu0Load、cpu1Load、cpu2Load、cpu3Load
   * @param count  曲线图点的个数
   * @param range  y轴范围
   */
  private initCpuLoad0to3Data(): LineData {

    let dataSet = new JArrayList<ILineDataSet>();

    let values = new JArrayList<EntryOhos>();
    for (let index: number = 0; index < this.gpData.length; index++) {
      const gpDataCur = this.gpData[index];
      if (gpDataCur.cpu0Load == '') {
        continue
      }
      values.add(new EntryOhos(Number(index).valueOf(), Number(gpDataCur.cpu0Load).valueOf()));
    }
    let gradientFillColor = [];
    gradientFillColor.push([0x0C0099CC, 0.2])
    gradientFillColor.push([0x7F0099CC, 0.4])
    gradientFillColor.push([0x0099CC, 1.0])
    let set1 = new LineDataSet(values, 'cpu0Load(%)');
    set1.setDrawFilled(false);
    set1.setMode(Mode.CUBIC_BEZIER);
    set1.setDrawValues(false);
    set1.setGradientFillColor(gradientFillColor)
    set1.setColorByColor(Color.Blue);
    set1.setLineWidth(3)
    set1.setDrawCircles(false);
    set1.setCircleColor(Color.Blue);
    set1.setCircleRadius(8);
    set1.setCircleHoleRadius(4)
    set1.setCircleHoleColor(Color.Blue)
    set1.setDrawCircleHole(false)
    dataSet.add(set1);

    let values2 = new JArrayList<EntryOhos>();
    for (let index: number = 0; index < this.gpData.length; index++) {
      const gpDataCur = this.gpData[index];
      if (gpDataCur.cpu1Load == '') {
        continue
      }
      values2.add(new EntryOhos(Number(index).valueOf(), Number(gpDataCur.cpu1Load).valueOf()));
    }
    let gradientFillColor2 = [];
    gradientFillColor2.push([0x0C0099CC, 0.2])
    gradientFillColor2.push([0x7F0099CC, 0.4])
    gradientFillColor2.push([0x0099CC, 1.0])
    let set2 = new LineDataSet(values2, 'cpu1Load(%)');
    set2.setDrawFilled(false);
    set2.setMode(Mode.CUBIC_BEZIER);
    set2.setDrawValues(false);
    set2.setGradientFillColor(gradientFillColor2)
    set2.setColorByColor(Color.Green);
    set2.setLineWidth(3)
    set2.setDrawCircles(false);
    set2.setCircleColor(Color.Green);
    set2.setCircleRadius(8);
    set2.setCircleHoleRadius(4)
    set2.setCircleHoleColor(Color.Green)
    set2.setDrawCircleHole(false)
    dataSet.add(set2);

    let values3 = new JArrayList<EntryOhos>();
    for (let index: number = 0; index < this.gpData.length; index++) {
      const gpDataCur = this.gpData[index];
      if (gpDataCur.cpu2Load == '') {
        continue
      }
      values3.add(new EntryOhos(Number(index).valueOf(), Number(gpDataCur.cpu2Load).valueOf()));
    }
    let gradientFillColor3 = [];
    gradientFillColor3.push([0x0C0099CC, 0.2])
    gradientFillColor3.push([0x7F0099CC, 0.4])
    gradientFillColor3.push([0x0099CC, 1.0])
    let set3 = new LineDataSet(values3, 'cpu2Load(%)');
    set3.setDrawFilled(false);
    set3.setMode(Mode.CUBIC_BEZIER);
    set3.setGradientFillColor(gradientFillColor3)
    set3.setColorByColor(Color.Red);
    set3.setLineWidth(3)
    set3.setDrawValues(false);
    set3.setDrawCircles(false);
    set3.setCircleColor(Color.Red);
    set3.setCircleRadius(8);
    set3.setCircleHoleRadius(4)
    set3.setCircleHoleColor(Color.Red)
    set3.setDrawCircleHole(false)
    dataSet.add(set3);

    let values4 = new JArrayList<EntryOhos>();
    for (let index: number = 0; index < this.gpData.length; index++) {
      const gpDataCur = this.gpData[index];
      if (gpDataCur.cpu3Load == '') {
        continue
      }
      values4.add(new EntryOhos(Number(index).valueOf(), Number(gpDataCur.cpu3Load).valueOf()));
    }
    let gradientFillColor4 = [];
    gradientFillColor4.push([0x0C0099CC, 0.2])
    gradientFillColor4.push([0x7F0099CC, 0.4])
    gradientFillColor4.push([0x0099CC, 1.0])
    let set4 = new LineDataSet(values4, 'cpu3Load(%)');
    set4.setDrawFilled(false);
    set1.setDrawValues(false);
    set4.setMode(Mode.CUBIC_BEZIER);
    set4.setGradientFillColor(gradientFillColor4)
    set4.setColorByColor(Color.Orange);
    set4.setLineWidth(3)
    set4.setDrawCircles(false);
    set4.setCircleColor(Color.Orange);
    set4.setCircleRadius(8);
    set4.setCircleHoleRadius(4)
    set4.setCircleHoleColor(Color.Orange)
    set4.setDrawCircleHole(false)
    dataSet.add(set4);

    return new LineData(dataSet)
  }

  private initCpu0FreData(): LineData {

    let values = new JArrayList<EntryOhos>();
    for (let index: number = 0; index < this.gpData.length; index++) {
      const gpDataCur = this.gpData[index];
      if (gpDataCur.cpu0Frequency == '') {
        continue
      }
      values.add(new EntryOhos(Number(index).valueOf(), Number(gpDataCur.cpu0Frequency).valueOf() / 1e3));
    }


    let gradientFillColor = [];
    gradientFillColor.push([0x0C0099CC, 0.2])
    gradientFillColor.push([0x7F0099CC, 0.4])
    gradientFillColor.push([0x0099CC, 1.0])

    let dataSet = new JArrayList<ILineDataSet>();

    let set1 = new LineDataSet(values, 'cpu0Frequency(MHZ)');
    set1.setDrawFilled(false);
    set1.setDrawValues(false);
    set1.setMode(Mode.CUBIC_BEZIER);
    set1.setGradientFillColor(gradientFillColor)
    set1.setColorByColor(Color.Red);
    set1.setLineWidth(3)
    set1.setDrawCircles(false);
    set1.setCircleColor(Color.Blue);
    set1.setCircleRadius(8);
    set1.setCircleHoleRadius(4)
    set1.setCircleHoleColor(Color.Red)
    set1.setDrawCircleHole(false)
    dataSet.add(set1);
    return new LineData(dataSet)
  }

  private initCpuLoad4to6Data(): LineData {

    let dataSet = new JArrayList<ILineDataSet>();

    let values = new JArrayList<EntryOhos>();
    for (let index: number = 0; index < this.gpData.length; index++) {
      const gpDataCur = this.gpData[index];
      if (gpDataCur.cpu4Load == '') {
        continue
      }
      values.add(new EntryOhos(Number(index).valueOf(), Number(gpDataCur.cpu4Load).valueOf()));
    }
    let gradientFillColor = [];
    gradientFillColor.push([0x0C0099CC, 0.2])
    gradientFillColor.push([0x7F0099CC, 0.4])
    gradientFillColor.push([0x0099CC, 1.0])
    let set1 = new LineDataSet(values, 'cpu4Load(%)');
    set1.setDrawFilled(false);
    set1.setMode(Mode.CUBIC_BEZIER);
    set1.setDrawValues(false);
    set1.setGradientFillColor(gradientFillColor)
    set1.setColorByColor(Color.Red);
    set1.setLineWidth(3)
    set1.setDrawCircles(false);
    set1.setCircleColor(Color.Red);
    set1.setCircleRadius(8);
    set1.setCircleHoleRadius(4)
    set1.setCircleHoleColor(Color.Red)
    set1.setDrawCircleHole(false)
    dataSet.add(set1);

    let values2 = new JArrayList<EntryOhos>();
    for (let index: number = 0; index < this.gpData.length; index++) {
      const gpDataCur = this.gpData[index];
      if (gpDataCur.cpu5Load == '') {
        continue
      }
      values2.add(new EntryOhos(Number(index).valueOf(), Number(gpDataCur.cpu5Load).valueOf()));
    }
    let gradientFillColor2 = [];
    gradientFillColor2.push([0x0C0099CC, 0.2])
    gradientFillColor2.push([0x7F0099CC, 0.4])
    gradientFillColor2.push([0x0099CC, 1.0])
    let set2 = new LineDataSet(values2, 'cpu5Load(%)');
    set2.setDrawFilled(false);
    set2.setMode(Mode.CUBIC_BEZIER);
    set2.setDrawValues(false);
    set2.setGradientFillColor(gradientFillColor2)
    set2.setColorByColor(Color.Blue);
    set2.setLineWidth(3)
    set2.setDrawCircles(false);
    set2.setCircleColor(Color.Blue);
    set2.setCircleRadius(8);
    set2.setCircleHoleRadius(4)
    set2.setCircleHoleColor(Color.Blue)
    set2.setDrawCircleHole(false)
    dataSet.add(set2);

    let values3 = new JArrayList<EntryOhos>();
    for (let index: number = 0; index < this.gpData.length; index++) {
      const gpDataCur = this.gpData[index];
      if (gpDataCur.cpu6Load == '') {
        continue
      }
      values3.add(new EntryOhos(Number(index).valueOf(), Number(gpDataCur.cpu6Load).valueOf()));
    }
    let gradientFillColor3 = [];
    gradientFillColor3.push([0x0C0099CC, 0.2])
    gradientFillColor3.push([0x7F0099CC, 0.4])
    gradientFillColor3.push([0x0099CC, 1.0])

    let set3 = new LineDataSet(values3, 'cpu6Load(%)');
    set3.setDrawFilled(false);
    set3.setDrawValues(false);
    set3.setMode(Mode.CUBIC_BEZIER);
    set3.setGradientFillColor(gradientFillColor3)
    set3.setColorByColor(Color.Green);
    set3.setLineWidth(3)
    set3.setDrawCircles(false);
    set3.setCircleColor(Color.Green);
    set3.setCircleRadius(8);
    set3.setCircleHoleRadius(4)
    set3.setCircleHoleColor(Color.Green)
    set3.setDrawCircleHole(false)
    dataSet.add(set3);

    return new LineData(dataSet)
  }

  private initCpuFre4Data(): LineData {

    let dataSet = new JArrayList<ILineDataSet>();

    let values = new JArrayList<EntryOhos>();
    for (let index: number = 0; index < this.gpData.length; index++) {
      const gpDataCur = this.gpData[index];
      if (gpDataCur.cpu4Frequency == '') {
        continue
      }
      values.add(new EntryOhos(Number(index).valueOf(), Number(gpDataCur.cpu4Frequency).valueOf() / 1e3));
    }
    let gradientFillColor = [];
    gradientFillColor.push([0x0C0099CC, 0.2])
    gradientFillColor.push([0x7F0099CC, 0.4])
    gradientFillColor.push([0x0099CC, 1.0])

    let set1 = new LineDataSet(values, 'cpu4Frequency(MHZ)');
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

  private initCpuLoad7Data(): LineData {

    let values = new JArrayList<EntryOhos>();
    for (let index: number = 0; index < this.gpData.length; index++) {
      const gpDataCur = this.gpData[index];
      if (gpDataCur.cpu7Load == '') {
        continue
      }
      values.add(new EntryOhos(Number(index).valueOf(), Number(gpDataCur.cpu7Load).valueOf()));
    }


    let gradientFillColor = [];
    gradientFillColor.push([0x0C0099CC, 0.2])
    gradientFillColor.push([0x7F0099CC, 0.4])
    gradientFillColor.push([0x0099CC, 1.0])

    let dataSet = new JArrayList<ILineDataSet>();

    let set1 = new LineDataSet(values, 'cpu7Load(%)');
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

  //CPUFre7
  private initCpuFre7Data(): LineData {

    let values = new JArrayList<EntryOhos>();
    for (let index: number = 0; index < this.gpData.length; index++) {
      const gpDataCur = this.gpData[index];
      if (gpDataCur.cpu7Frequency == '') {
        continue
      }
      values.add(new EntryOhos(Number(index).valueOf(), Number(gpDataCur.cpu7Frequency).valueOf() / 1e3));
    }


    let gradientFillColor = [];
    gradientFillColor.push([0x0C0099CC, 0.2])
    gradientFillColor.push([0x7F0099CC, 0.4])
    gradientFillColor.push([0x0099CC, 1.0])

    let dataSet = new JArrayList<ILineDataSet>();

    let set1 = new LineDataSet(values, 'cpu7Frequency(MHZ)');
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

  //GPULoad
  private initGpuLoadData(): LineData {

    let values = new JArrayList<EntryOhos>();
    for (let index: number = 0; index < this.gpData.length; index++) {
      const gpDataCur = this.gpData[index];
      if (gpDataCur.gpuLoad == '') {
        continue
      }
      values.add(new EntryOhos(Number(index).valueOf(), Number(gpDataCur.gpuLoad).valueOf()));
    }


    let gradientFillColor = [];
    gradientFillColor.push([0x0C0099CC, 0.2])
    gradientFillColor.push([0x7F0099CC, 0.4])
    gradientFillColor.push([0x0099CC, 1.0])

    let dataSet = new JArrayList<ILineDataSet>();

    let set1 = new LineDataSet(values, 'GPULoad(%)');
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

  private initGPUFreData(): LineData {

    let values = new JArrayList<EntryOhos>();
    for (let index: number = 0; index < this.gpData.length; index++) {
      const gpDataCur = this.gpData[index];
      if (gpDataCur.gpuFrequency == '') {
        continue
      }
      values.add(new EntryOhos(Number(index).valueOf(), Number(gpDataCur.gpuFrequency).valueOf() / 1e6));
    }


    let gradientFillColor = [];
    gradientFillColor.push([0x0C0099CC, 0.2])
    gradientFillColor.push([0x7F0099CC, 0.4])
    gradientFillColor.push([0x0099CC, 1.0])

    let dataSet = new JArrayList<ILineDataSet>();

    let set1 = new LineDataSet(values, 'GPUFrequency(MHZ)');
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

  private initDDRFreData(): LineData {

    let values = new JArrayList<EntryOhos>();
    for (let index: number = 0; index < this.gpData.length; index++) {
      const gpDataCur = this.gpData[index];
      if (gpDataCur.ddrFrequency == '') {
        continue
      }
      values.add(new EntryOhos(Number(index).valueOf(), Number(gpDataCur.ddrFrequency).valueOf() / 1e6));
    }


    let gradientFillColor = [];
    gradientFillColor.push([0x0C0099CC, 0.2])
    gradientFillColor.push([0x7F0099CC, 0.4])
    gradientFillColor.push([0x0099CC, 1.0])

    let dataSet = new JArrayList<ILineDataSet>();

    let set1 = new LineDataSet(values, 'DDR Frequency(MHZ)');
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

    Stack({ alignContent: Alignment.TopStart }) {
      Scroll() {
        Column({ space: 20 }) {
          LineChart({lineChartModel: this.lineChartModel})
          Row({ space: 20 }) {
            Text('cpu0Load(%),') {
            }.fontWeight(FontWeight.Bold).fontColor(Color.Blue).fontSize('15fp').textAlign(TextAlign.Center)
            Text('cpu1Load(%),').fontWeight(FontWeight.Bold).fontColor(Color.Green).fontSize('15fp').textAlign(TextAlign.Center)
            Text('cpu2Load(%),').fontWeight(FontWeight.Bold).fontColor(Color.Red).fontSize('15fp').textAlign(TextAlign.Center)
            Text('cpu3Load(%)').fontWeight(FontWeight.Bold).fontColor(Color.Orange).fontSize('15fp').textAlign(TextAlign.Center)
          }
          LineChart({lineChartModel: this.lineChartModel1})
          Text('cpu0Frequency(MHZ)') {
          }.fontWeight(FontWeight.Bold).fontColor(Color.Red).fontSize('15fp').textAlign(TextAlign.Center)
          LineChart({lineChartModel: this.lineChartModel2})
          Row({ space: 20 }) {
            Text('cpu4Load(%),').fontWeight(FontWeight.Bold).fontColor(Color.Red).fontSize('15fp').textAlign(TextAlign.Center)
            Text('cpu5Load(%),') {
            }.fontWeight(FontWeight.Bold).fontColor(Color.Blue).fontSize('15fp').textAlign(TextAlign.Center)
            Text('cpu6Load(%),').fontWeight(FontWeight.Bold).fontColor(Color.Green).fontSize('15fp').textAlign(TextAlign.Center)
          }
          LineChart({lineChartModel: this.lineChartModel3})
          Text('cpu4Frequency(MHZ)') {
          }.fontWeight(FontWeight.Bold).fontColor(Color.Green).fontSize('15fp').textAlign(TextAlign.Center)

          LineChart({lineChartModel: this.lineChartModel4})
          Text('cpu7Load(%)') {
          }.fontWeight(FontWeight.Bold).fontColor(Color.Green).fontSize('15fp').textAlign(TextAlign.Center)

          LineChart({lineChartModel: this.lineChartModel5})
          Text('cpu7Frequency(MHZ)') {
          }.fontWeight(FontWeight.Bold).fontColor(Color.Green).fontSize('15fp').textAlign(TextAlign.Center)


          LineChart({lineChartModel: this.lineChartModel6})
          Text('GPULoad(%)') {
          }.fontWeight(FontWeight.Bold).fontColor(Color.Green).fontSize('15fp').textAlign(TextAlign.Center)

          LineChart({lineChartModel: this.lineChartModel7})
          Text('GPUFrequency(MHZ)') {
          }.fontWeight(FontWeight.Bold).fontColor(Color.Green).fontSize('15fp').textAlign(TextAlign.Center)

          Stack() {
            Column() {
              LineChart({lineChartModel: this.lineChartModel8})
              Text('DDR Frequency(MHZ)') {
              }.fontWeight(FontWeight.Bold).fontColor(Color.Green).fontSize('15fp').textAlign(TextAlign.Center).margin({ top: '50%' })
            }
          }
        }.width('100%').alignItems(HorizontalAlign.Center)
      }.width('100%')
    }.width('100%').height('100%')
  }
}
