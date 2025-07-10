/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

import type { RawSourceMap } from 'typescript';
import { SourceMap } from 'magic-string';
import { SourceMapSegment, decode } from '@jridgewell/sourcemap-codec';
import assert from 'assert';

enum SegmentIndex {
  ORIGINAL_COLUMN_INDEX = 0,
  SOURCE_INDEX = 1,
  TRANSFORMED_LINE_INDEX = 2,
  TRANSFORMED_COLUMN_INDEX = 3,
  NAME_INDEX = 4,
}

/**
 * The sourcemap format with decoded mappings with number type.
 */
export interface ExistingDecodedSourceMap {
  file?: string;
  mappings: SourceMapSegment[][];
  names?: string[];
  sourceRoot?: string;
  sources: string[];
  sourcesContent?: string[];
  version: number;
}

interface BaseSource {
  traceSegment(line: number, column: number, name: string): SourceMapSegmentObj | null;
}

/**
 * The source file info.
 */
export class Source implements BaseSource {
  readonly content: string | null;
  readonly filename: string;
  isOriginal = true;

  constructor(filename: string, content: string | null) {
    this.filename = filename;
    this.content = content;
  }

  traceSegment(line: number, column: number, name: string): SourceMapSegmentObj {
    return { column, line, name, source: this };
  }
}

/**
 * The interpreted sourcemap line and column info.
 */
export interface SourceMapSegmentObj {
  column: number;
  line: number;
  name: string;
  source: Source;
}

type MappingsNameType = { mappings: readonly SourceMapSegment[][]; names?: readonly string[] };
type TracedMappingsType = { mappings: SourceMapSegment[][]; names: string[]; sources: string[] };

/**
 * Type of the map parameter of the SourceMapLink class.
 */
export type MappingsNameTypeForTest = MappingsNameType;

/**
 * Provide api tools related to sourcemap.
 */
export class SourceMapLink implements BaseSource {
  readonly mappings: readonly SourceMapSegment[][];
  readonly names?: readonly string[];
  readonly sources: BaseSource[];

  constructor(map: MappingsNameType, sources: BaseSource[]) {
    this.sources = sources;
    this.names = map.names;
    this.mappings = map.mappings;
  }

  traceMappings(): TracedMappingsType {
    const tracedSources: string[] = [];
    const sourceIndexMap = new Map<string, number>();
    const sourcesContent: (string | null)[] = [];
    const tracednames: string[] = [];
    const nameIndexMap = new Map<string, number>();

    const mappings = [];

    for (const line of this.mappings) {
      const tracedLine: SourceMapSegment[] = [];

      for (const segment of line) {
        if (segment.length === 1) { // The number of elements is insufficient.
          continue;
        }
        const source = this.sources[segment[SegmentIndex.SOURCE_INDEX]];
        if (!source) {
          continue;
        }
        // segment[2] records the line number of the code before transform, segment[3] records the column number of the code before transform.
        // segment[4] records the name from the names array.
        assert(segment.length >= 4, 'The length of the mapping segment is incorrect.');
        let line: number = segment[SegmentIndex.TRANSFORMED_LINE_INDEX];
        let column: number = segment[SegmentIndex.TRANSFORMED_COLUMN_INDEX];
        // If the length of the segment is 5, it will have name content.
        let name: string = segment.length === 5 ? this.names[segment[SegmentIndex.NAME_INDEX]] : '';
        const traced = source.traceSegment(line, column, name);

        if (traced) {
          this.analyzeTracedSource(traced, tracedSources, sourceIndexMap, sourcesContent);
          let sourceIndex = sourceIndexMap.get(traced.source.filename);
          const targetSegment: SourceMapSegment = [segment[SegmentIndex.ORIGINAL_COLUMN_INDEX], sourceIndex, traced.line, traced.column];
          this.recordTracedName(traced, tracednames, nameIndexMap, targetSegment);
          tracedLine.push(targetSegment);
        }
      }

      mappings.push(tracedLine);
    }

    return { mappings, names: tracednames, sources: tracedSources };
  }

  analyzeTracedSource(traced: SourceMapSegmentObj, tracedSources: string[], sourceIndexMap: Map<string, number>, sourcesContent: (string | null)[]): void {
    const content = traced.source.content;
    const filename = traced.source.filename;
    // Get the source index from sourceIndexMap, which is the second element of sourcemap.
    let sourceIndex = sourceIndexMap.get(filename);
    if (sourceIndex === undefined) {
      sourceIndex = tracedSources.length;
      tracedSources.push(filename);
      sourceIndexMap.set(filename, sourceIndex);
      sourcesContent[sourceIndex] = content;
    } else if (sourcesContent[sourceIndex] == null) { // Update text when content is empty.
      sourcesContent[sourceIndex] = content;
    } else if (content != null && sourcesContent[sourceIndex] !== content) {
      throw new Error(`Multiple conflicting contents for sourcemap source: ${filename}`);
    }
  }

  recordTracedName(traced: SourceMapSegmentObj, tracednames: string[], nameIndexMap: Map<string, number>, targetSegment: SourceMapSegment): void {
    if (traced.name) {
      const name = traced.name;
      let nameIndex = nameIndexMap.get(name);
      if (nameIndex === undefined) {
        nameIndex = tracednames.length;
        tracednames.push(name);
        nameIndexMap.set(name, nameIndex);
      }
      // Add the fourth element: name position
      targetSegment.push(nameIndex);
    }
  }

  traceSegment(line: number, column: number, name: string): SourceMapSegmentObj | null {
    const segments = this.mappings[line];
    if (!segments) {
      return null;
    }

    // Binary search segment for the target columns.
    let binarySearchStart = 0;
    let binarySearchEnd = segments.length - 1; // Get the last elemnt index.

    while (binarySearchStart <= binarySearchEnd) {
      // Calculate the intermediate index.
      const m = (binarySearchStart + binarySearchEnd) >> 1;
      const tempSegment = segments[m];
      let tempColumn = tempSegment[SegmentIndex.ORIGINAL_COLUMN_INDEX];
      // If a sourcemap does not have sufficient resolution to contain a necessary mapping, e.g. because it only contains line information, we
      // use the best approximation we could find
      if (tempColumn === column || binarySearchStart === binarySearchEnd) {
        if (tempSegment.length === 1) { // The number of elements is insufficient.
          return null;
        }
        const tracedSource = tempSegment[SegmentIndex.SOURCE_INDEX];
        const source = this.sources[tracedSource];
        if (!source) {
          return null;
        }

        let tracedLine: number = tempSegment[SegmentIndex.TRANSFORMED_LINE_INDEX];
        let tracedColumn: number = tempSegment[SegmentIndex.TRANSFORMED_COLUMN_INDEX];
        let tracedName: string = tempSegment.length === 5 ? this.names[tempSegment[SegmentIndex.NAME_INDEX]] : name;
        return source.traceSegment(tracedLine, tracedColumn, tracedName);
      }
      if (tempColumn > column) {
        // Target is in the left half
        binarySearchEnd = m - 1;
      } else {
        // Target is in the right half
        binarySearchStart = m + 1;
      }
    }

    return null;
  }
}

/**
 * Decode the sourcemap from string format to number format.
 * @param map The sourcemap with raw string format, eg. mappings: IAGS,OAAO,GAAE,MAAM,CAAA;
 * @returns The sourcemap with decoded number format, eg. mappings: [4,0,3,9], [7,0,0,7], [3,0,0,2], [6,0,0,6], [1,0,0,0]
 */
export function decodeSourcemap(map: RawSourceMap): ExistingDecodedSourceMap | null {
  if (!map) {
    return null;
  }
  if (map.mappings === '') {
    return { mappings: [], names: [], sources: [], version: 3 }; // 3 is the sourcemap version.
  }
  const mappings: SourceMapSegment[][] = decode(map.mappings);
  return { ...map, mappings: mappings };
}

function generateChain(sourcemapChain: ExistingDecodedSourceMap[], map: RawSourceMap): void {
  sourcemapChain.push(decodeSourcemap(map));
}

/**
 * Merge the sourcemaps of the two processes into the sourcemap of the complete process.
 * @param previousMap The sourcemap before obfuscation process, such as ets-loader transform
 * @param currentMap The sourcemap of obfuscation process
 * @returns The merged sourcemap
 */
export function mergeSourceMap(previousMap: RawSourceMap, currentMap: RawSourceMap): RawSourceMap {
  const sourcemapChain: ExistingDecodedSourceMap[] = [];
  // The ets-loader esmodule mode processes one file at a time, so get the file name at index 1
  const sourceFileName = previousMap.sources.length === 1 ? previousMap.sources[0] : '';
  const source: Source = new Source(sourceFileName, null);
  generateChain(sourcemapChain, previousMap);
  generateChain(sourcemapChain, currentMap);
  const collapsedSourcemap: SourceMapLink = sourcemapChain.reduce(
    (source: BaseSource, map: ExistingDecodedSourceMap): SourceMapLink => {
      return new SourceMapLink(map, [source]);
    },
    source,
  ) as SourceMapLink;
  const tracedMappings: TracedMappingsType = collapsedSourcemap.traceMappings();
  const result: RawSourceMap = new SourceMap({ ...tracedMappings, file: previousMap.file }) as RawSourceMap;
  result.sourceRoot = previousMap.sourceRoot;
  return result;
}
