package(default_visibility = ["//visibility:public"])

cc_library(
    name = "array",
    hdrs = ["array.h"],
)

cc_library(
    name = "static_map",
    hdrs = ["static_map.h"],
    deps = [
        ":array",
        "//base:logging",
    ],
)

cc_test(
    name = "static_map_test",
    srcs = ["static_map_test.cc"],
    deps = [
        ":static_map",
        "//utils/testing:test_main",
    ],
)
