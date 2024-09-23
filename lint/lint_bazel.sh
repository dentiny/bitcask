#! /usr/bin/env bash

bazel run //:buildifier
bazel-bin/buildifier.bash.runfiles/com_github_bazelbuild_buildtools/buildifier/buildifier_/buildifier -r .
