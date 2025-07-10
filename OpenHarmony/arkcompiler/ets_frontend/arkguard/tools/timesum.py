#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Copyright (c) 2025 Huawei Device Co., Ltd.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

from collections import defaultdict
import json
import logging
import os
import stat

# Configure logging to display timestamps and messages
logging.basicConfig(level=logging.INFO, format="%(asctime)s - %(message)s")


def gen_parent(data):
    """
    Generate parent information for each event in the data.
    This function sorts the events by timestamp and assigns a parent to each event
    based on the conditions that the parent event must fully contain the current event
    in terms of start time (ts) and duration (dur), and share the same process (pid) and thread (tid).
    """
    # Sort the data by timestamp (ts)
    data = sorted(data, key=lambda x: x["ts"])
    data_length = len(data)
    for i in range(data_length):
        current = data[i]  # Current event
        current_end = current["ts"] + current["dur"]  # End time of the current event
        for j in range(i - 1, -1, -1):  # Candidate parent event
            candidate = data[j]
            if candidate is None:
                continue
            candidate_end = candidate["ts"] + candidate["dur"]
            # Check if the candidate event fully contains the current event
            if (
                candidate["ts"] <= current["ts"]
                and candidate_end >= current_end
                and candidate["pid"] == current["pid"]
                and candidate["tid"] == current["tid"]
            ):
                # Assign the candidate event as the parent of the current event
                current["parent"] = candidate["name"]
                break
        if "parent" in current and current["name"] == current["parent"]:
            data[i] = None

    data = [event for event in data if event is not None]
    return data


def re_gen_ts_and_dur(data_with_parent):
    """
    Regenerate timestamps (ts) and durations (dur) for events based on their parent relationships.
    This function aggregates durations for events with the same parent and name,
    and adjusts their timestamps to ensure correct ordering.
    """
    # Dictionary to store aggregated data
    data_with_dur = {}
    for event in data_with_parent:
        name = event["name"]
        parent = event.get("parent")
        key = (parent, name)  # Use parent and name as a composite key
        ts = int(event["ts"])
        dur = int(event["dur"])
        end = ts + dur
        if key not in data_with_dur:
            # Initialize the entry if the key doesn't exist
            data_with_dur[key] = {
                **event,
                "ts": ts,
                "dur": dur,
                "end": end,
            }
        else:
            # Aggregate durations for events with the same parent and name
            data_with_dur[key]["dur"] += dur
            data_with_dur[key]["end"] = (
                data_with_dur[key]["ts"] + data_with_dur[key]["dur"]
            )

    # Convert the dictionary back to a list and sort by timestamp
    re_data = list(data_with_dur.values())
    re_data.sort(key=lambda x: x["ts"])

    # Group events by their parent
    groups = group_by_parent(re_data)

    def get_parent_ts(parent_name, re_data):
        """
        Helper function to get the start time (ts) of a parent event.
        Handles cases where the parent is a root node (no parent) or a non-root node.
        """
        if not parent_name:
            return None  # Root node case
        for event in re_data:
            # Get the start time of the root node
            if event["name"] == parent_name and "parent" not in event:
                return event["ts"]
        # Get the start time of a non-root node as a parent
        for event in re_data:
            if event["name"] == parent_name:
                return event["ts"]
        return None

    # Flatten the grouped data and adjust timestamps based on parent relationships
    flattened = []
    for parent_name, nodes in groups.items():
        if not nodes:
            continue
        nodes.sort(key=lambda x: x["ts"])
        parent_ts = get_parent_ts(parent_name, re_data)
        if parent_ts is not None:
            # Adjust timestamps based on the parent's start time
            nodes[0]["ts"] = parent_ts
            nodes[0]["end"] = nodes[0]["ts"] + nodes[0]["dur"]
            for i in range(1, len(nodes)):
                nodes[i]["ts"] = nodes[i - 1]["end"]
                nodes[i]["end"] = nodes[i]["ts"] + nodes[i]["dur"]
        else:
            # Adjust timestamps sequentially if no parent start time is available
            current_end = nodes[0]["ts"] + nodes[0]["dur"]
            for i in range(1, len(nodes)):
                nodes[i]["ts"] = current_end
                current_end = nodes[i]["ts"] + nodes[i]["dur"]
                nodes[i]["end"] = current_end
        flattened.extend(nodes)

    # Save the flattened data to a JSON file
    if os.path.exists('data.json'):
        os.remove('data.json')
    flags = os.O_WRONLY | os.O_CREAT | os.O_EXCL
    modes = stat.S_IWUSR | stat.S_IRUSR
    with os.fdopen(os.open('data.json', flags, modes), 'w') as file:
        json.dump(flattened, file, indent=2)
    return groups


def group_by_parent(data):
    """
    Group events by their parent.
    Events with the same parent are grouped together and sorted by timestamp.
    """
    groups = defaultdict(list)
    for node in data:
        parent = node.get("parent")
        groups[parent].append(node)
    for parent in groups:
        groups[parent].sort(key=lambda x: x["ts"])
    return dict(groups)


if __name__ == "__main__":
    # Load input data from a JSON file
    with open("timePerformanceData.json", "r") as f:
        datas = json.load(f)
        # Generate parent information for each event
        data_with_parents = gen_parent(datas)
        # Regenerate timestamps and durations based on parent relationships
        re_gen_ts_and_dur(data_with_parents)
