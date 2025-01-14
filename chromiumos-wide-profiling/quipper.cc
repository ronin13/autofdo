// Copyright (c) 2012 The Chromium OS Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <sstream>
#include <string>

#include "base/logging.h"

#include "chromiumos-wide-profiling/perf_protobuf_io.h"
#include "chromiumos-wide-profiling/perf_reader.h"
#include "chromiumos-wide-profiling/perf_recorder.h"
#include "chromiumos-wide-profiling/quipper_string.h"

namespace {

const char kDefaultOutputFile[] = "/dev/stdout";

int StringToInt(const string& s) {
  int r;
  stringstream ss;
  ss << s;
  ss >> r;
  return r;
}

bool ParseArguments(int argc, char* argv[],
                    std::vector<string>* perf_args,
                    int* duration) {
  if (argc < 3) {
    LOG(ERROR) << "Invalid command line.";
    LOG(ERROR) << "Usage: " << argv[0] <<
               " <duration in seconds>" <<
               " <path to perf>" <<
               " <perf arguments>";
    return false;
  }

  *duration = StringToInt(argv[1]);

  for (int i = 2; i < argc; i++) {
    perf_args->emplace_back(argv[i]);
  }
  return true;
}

}  // namespace

// Usage is:
// <exe> <duration in seconds> <perf command line>
int main(int argc, char* argv[]) {
  std::vector<string> perf_args;
  int perf_duration;

  if (!ParseArguments(argc, argv, &perf_args, &perf_duration))
    return 1;

  quipper::PerfRecorder perf_recorder;
  quipper::PerfDataProto perf_data_proto;
  if (!perf_recorder.RecordAndConvertToProtobuf(perf_args,
                                                perf_duration,
                                                &perf_data_proto))
    return 1;
  if (!WriteProtobufToFile(perf_data_proto, kDefaultOutputFile))
    return 1;
  return 0;
}
