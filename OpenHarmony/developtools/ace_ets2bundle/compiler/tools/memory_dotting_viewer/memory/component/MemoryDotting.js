/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
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
import {BaseElement} from '../../base-ui/BaseElement.js';
import {memoryDottingHtml} from './MemoryDottingHtml.html.js';

export class MemoryDotting extends BaseElement {
  constructor() {
    super();
    this.memoryDataPoints = [];
    this.sourceData = [];
  }

  set data(datas) {
    let context = this.canvas.getContext('2d', {alpha: true});
    context.clearRect(0, 0, this.canvas.width, this.canvas.height);
    let firstTimestamp = datas[0].timestamp;
    let processedData = datas.map(item => ({
        timestamp: item.timestamp - firstTimestamp,
        rss: this.convertToMB(item.rss),
        stage: item.stage,
        parentStage: item.parentStage
    }));
    this.sourceData = processedData;
    this.drawChart(context, processedData);
  }
  convertToMB(value) {
	return value / (1024 * 1024);
  };

  initElements() {
    this.canvas = this.shadowRoot.querySelector('#canvas');
    this.toolTip = this.shadowRoot.querySelector('#memory-tool-tip');
  }

  initHtml() {
    return memoryDottingHtml;
  }

  connectedCallback() {
    super.connectedCallback();
    new ResizeObserver(() => {
      this.parentElement.style.overflow = 'hidden';
    }).observe(this.parentElement);
  }

  disconnectedCallback() {
    super.disconnectedCallback();
  }

  showTooltip(event, value, key, time, stage, parentStage) {
    const tooltipRect = this.toolTip.getBoundingClientRect();
    let tooltipLeft = event.clientX - (tooltipRect.width + 100);
    let tooltipTop = event.clientY - tooltipRect.height - 10;
    if (tooltipLeft + tooltipRect.width > window.innerWidth) {
      tooltipLeft = event.clientX - tooltipRect.width - 5;
    } else if (tooltipLeft < 0) {
      tooltipLeft = event.clientX + 5;
    }
    if (tooltipTop < 0) {
      tooltipTop = event.clientY + 5;
    }
    this.toolTip.style.visibility = 'visible';
    this.toolTip.innerHTML = `X: ${time}<br>Y: ${value}(MB)<br>Type: ${key}<br>stage: ${stage}<br>parentStage: ${parentStage}`;
    this.toolTip.style.left = tooltipLeft + 'px';
    this.toolTip.style.top = tooltipTop + 'px';
  }

  hideTooltip() {
    this.toolTip.style.visibility = 'hidden';
  }

  isInsideCircle(mx, my, cx, cy, radius = 5) {
    return Math.sqrt(Math.pow((mx - cx), 2) + Math.pow((my - cy), 2)) <= radius;
  }

  static isLocalMaxOrMin(curr, prev, next, threshold) {
    const rssDiffPrev = Math.abs(curr.rss - prev.rss);
    const rssDiffNext = Math.abs(curr.rss - next.rss);
    return (
      (curr.rss > prev.rss && curr.rss > next.rss && rssDiffPrev >= threshold && 
      rssDiffNext >= threshold) || (curr.rss < prev.rss && curr.rss < next.rss && rssDiffPrev >= 
      threshold && rssDiffNext >= threshold)
    );
  }

  static isTurningPoint(curr, prev, next, threshold) {
    const rssDiffPrev = Math.abs(curr.rss - prev.rss);
    const rssDiffNext = Math.abs(curr.rss - next.rss);
    return (
      (prev.rss < curr.rss && curr.rss > next.rss && rssDiffPrev >= threshold && rssDiffNext >= threshold) ||
      (prev.rss > curr.rss && curr.rss < next.rss && rssDiffPrev >= threshold && rssDiffNext >= threshold)
    );
  }

  static filterKeyPoints(data, threshold = 500) {
    if (data.length <= 2) {
      return data;
    }
    let filteredData = [data[0]];
    for (let i = 1; i < data.length - 1; i++) {
      const prev = data[i - 1];
      const curr = data[i];
      const next = data[i + 1];
      if (MemoryDotting.isLocalMaxOrMin(curr, prev, next, threshold) ||
      MemoryDotting.isTurningPoint(curr, prev, next, threshold)) {
        filteredData.push(curr);
      }
  }
  filteredData.push(data[data.length - 1]);
  return filteredData;
  }


  getMaxValue() {
    let rssValues = this.sourceData.map(item => item.rss);
    let maxValue = Math.max(...rssValues);
    return maxValue;
  }

  getMinValue() {
    let rssValues = this.sourceData.map(item => item.rss);
    let minValue = Math.min(...rssValues);
    return minValue;
  }

  drawChart(ctx, data) {
    if (!ctx) {
      return;
    }

    const {width, height, padding} = this.getCanvasDimensions();
    ctx.clearRect(0, 0, width, height);
    const {timestamps, rssValues} = this.extractData(data);
    const {maxVal, minVal} = this.getMinMaxValues();
    this.drawGridLines(ctx, width, height, padding);
    this.drawAxis(ctx, width, height, padding, maxVal, minVal, timestamps);
    this.addEventListeners(ctx, data);
    this.drawDataLines(ctx, width, height, padding, maxVal, minVal, timestamps, rssValues, data);
    this.addLegend(ctx, width, height, padding);
  }

  getCanvasDimensions() {
    return {
      width: this.canvas.clientWidth,
      height: this.canvas.clientHeight,
      padding: 50
    };
  }

  extractData(data) {
    return {
      timestamps: data.map(item => item.timestamp),
      rssValues: data.map(item => item.rss)
    };
  }

  getMinMaxValues() {
    return {
      maxVal: this.getMaxValue(),
      minVal: this.getMinValue()
    };
  }

  drawGridLines(ctx, width, height, padding) {
    ctx.strokeStyle = '#ccc';
    ctx.lineWidth = 1;
    for (let x = padding; x < width - padding; x += (width - 2 * padding) / 5) {
      ctx.beginPath();
      ctx.moveTo(x, padding);
      ctx.lineTo(x, height - padding);
      ctx.stroke();
    }
  }

  drawAxis(ctx, width, height, padding, maxVal, minVal, timestamps) {
    const numTicks = 5;
    const tickStep = (maxVal - minVal) / numTicks;
    for (let i = 0; i <= numTicks; i++) {
      const y = height - padding - (height - 2 * padding) * (i * tickStep) / (maxVal - minVal);
      ctx.beginPath();
      ctx.moveTo(padding, y);
      ctx.lineTo(width - padding, y);
      ctx.stroke();
    }

    ctx.beginPath();
    ctx.moveTo(padding, height - padding);
    ctx.lineTo(width - padding, height - padding);
    ctx.strokeStyle = '#000';
    ctx.lineWidth = 2;
    ctx.stroke();

    ctx.fillStyle = '#666';
	let firstTimestamp = timestamps[0];
	let lastTimestamp = timestamps[timestamps.length - 1];
	let timeInterval = (lastTimestamp - firstTimestamp) / 8;

	for (let i = 0; i <= 8; i++) {
		let timestamp = firstTimestamp + i * timeInterval;
		let x = padding + ((timestamp - firstTimestamp) / (lastTimestamp - firstTimestamp)) * (width - 2 * padding);
		this.drawVerticalText(ctx, this.formatTimestamp(timestamp), x, height - padding + 20, -90);
	}

    ctx.beginPath();
    ctx.moveTo(padding, padding);
    ctx.lineTo(padding, height - padding);
    ctx.strokeStyle = '#000';
    ctx.lineWidth = 2;
    ctx.stroke();

    for (let i = 0; i <= numTicks; i++) {
      const y = height - padding - (height - 2 * padding) * (i * tickStep) / (maxVal - minVal);
      ctx.fillText((minVal + i * tickStep).toFixed(0), padding - 20, y);
    }
  }

  addEventListeners(ctx, data) {
    this.canvas.addEventListener('mousemove', (event) => {
      const rect = this.canvas.getBoundingClientRect();
      const mouseX = event.clientX - rect.left;
      const mouseY = event.clientY - rect.top;
      for (const dataPoint of this.memoryDataPoints) {
        if (this.isInsideCircle(mouseX, mouseY, dataPoint.x, dataPoint.y)) {
          this.showTooltip(event, dataPoint.value, dataPoint.key, dataPoint.time, dataPoint.stage, dataPoint.parentStage);
          return;
        }
      }
      this.hideTooltip();
    });
    this.canvas.addEventListener('mouseout', () => {
      this.hideTooltip();
    });
  }

  drawDataLines(ctx, width, height, padding, maxVal, minVal, timestamps, rssValues, data) {
    const drawLine = (ctx, values, color, label) => {
      ctx.lineWidth = 1;
      ctx.strokeStyle = color;
      ctx.beginPath();
      this.drawLine(ctx, timestamps, values, padding, width, height, minVal, maxVal, label, data);
      ctx.stroke();
    };
    drawLine(ctx, rssValues, 'red', 'RSS');
  }

  addLegend(ctx, width, height, padding) {
    ctx.fillStyle = '#000';
    ctx.fillText('RSS', width - 100, padding + 20);
    ctx.fillStyle = 'red';
    ctx.fillRect(width - 120, padding + 15, 10, 10);
  }

  drawLine(ctx, timestamps, values, padding, width, height, minVal, maxVal, type, data) {
    ctx.beginPath();
    let pointX = padding;
    let pointY = height - padding - (values[0] - minVal) * (height - 2 * padding) / (maxVal - minVal);
    for (let i = 1; i < values.length; i++) {
      ctx.moveTo(pointX, pointY);
      if (i === 1) {
        ctx.arc(pointX, pointY, 4, 0, Math.PI * 2);
        this.memoryDataPoints.push({
          x: pointX, y: pointY, value: values[0], key: type,
          time: this.formatTimestampTitle(timestamps[0]), stage: data[0].stage, parentStage: data[0].parentStage
        });
        ctx.fillStyle = ctx.strokeStyle;
        ctx.fill();
      }
      const x = padding + ((timestamps[i] - timestamps[0]) / (timestamps[timestamps.length - 1] - timestamps[0])) * (width - 2 * padding);
      const y = height - padding - (values[i] - minVal) * (height - 2 * padding) / (maxVal - minVal);
      ctx.lineTo(x, y);
      ctx.arc(x, y, 4, 0, Math.PI * 2);
      this.memoryDataPoints.push({
        x: x, y: y, value: values[i], key: type,
        time: this.formatTimestampTitle(timestamps[i]), stage: data[i].stage, parentStage: data[i].parentStage
      });
      ctx.fillStyle = ctx.strokeStyle;
      ctx.fill();
      pointX = x;
      pointY = y;
    }
    ctx.closePath();
  }

  formatTimestamp(timestamp) {
	return timestamp.toFixed(2) + 'ms';
  }

  formatTimestampTitle(timestamp) {
	return timestamp + '(ms)';
}

  drawVerticalText(ctx, text, x, y, rotationAngle) {
    ctx.save();
    ctx.textBaseline = 'middle';
    ctx.textAlign = 'center';
    ctx.translate(x, y);
    ctx.rotate(rotationAngle * Math.PI / 180);
    ctx.fillText(text, 0, 0);
    ctx.restore();
  }
}

if (!customElements.get('memory-dotting')) {
  customElements.define('memory-dotting', MemoryDotting);
}