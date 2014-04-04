// Copyright 2014 Google Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Main function to create AutoFDO Profile.
//
// To test if this tool is working properly with perflab:
//   1. build create_gcov: blaze build devtools/crosstoo/autofdo:create_gcov
//   2. perflab checkout k8
//   3. perflab -k --fdo=gen-afdo build k8
//   4. perflab qfetch `perflab -k --arch=ikaria_westmere \
//                      --fdo=gen-afdo qsub k8 | awk '{print $3}'`
//   5. perflab -k --arch=ikaria_westmere --create_gcov=your_create_gcov_path \
//              --fdo_profile_root=. --fdo_profile_name=name create-autoprof k8
//   6. perflab -k --fdo_profile_root=. --fdo_profile_name=name --fdo=use-afdo \
//              build k8
//   7. perflab qfetch `perflab -k --label=opt --arch=ikaria_westmere qsub k8 \
//                      | awk '{print $3}'`
//   8. perflab --arch=ikaria_westmere --label=opt table k8

#include "gflags/gflags.h"
#include "profile_creator.h"

DEFINE_string(profile, "perf.data",
              "Profile file name");
DEFINE_string(profiler, "perf",
              "Profile type");
DEFINE_string(gcov, "fbdata.afdo",
              "Output file name");
DEFINE_string(binary, "data.binary",
              "Binary file name");

int main(int argc, char **argv) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);

  autofdo::ProfileCreator creator(FLAGS_binary);
  if (creator.CreateProfile(FLAGS_profile, FLAGS_profiler, FLAGS_gcov,
                            "gcov")) {
    return 0;
  } else {
    return -1;
  }
}