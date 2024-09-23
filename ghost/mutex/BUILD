package(default_visibility = ["//visibility:public"])

cc_library(
    name = "thread_annotations",
    hdrs = ["thread_annotations.h"],
    deps = [
        "//ghost/mutex/internal:thread_annotations",
    ],
)

cc_library(
    name = "annotated_mutex",
    hdrs = ["annotated_mutex.h"],
    deps = [
        ":thread_annotations",
        "//ghost/mutex/internal:mutex_impl",
    ],
)

cc_library(
    name = "annotated_lock",
    hdrs = ["annotated_lock.h"],
    deps = [
        ":thread_annotations",
        "//ghost/mutex/internal:mutex_impl",
    ],
)

cc_library(
    name = "mutex",
    hdrs = ["mutex.h"],
    deps = [
        ":annotated_lock",
        ":annotated_mutex",
    ],
)

cc_test(
    name = "mutex_test",
    srcs = ["mutex_test.cc"],
    deps = [
        ":mutex",
        "//base:init_ghost",
        "@gtest//:gtest_main",
    ],
)
