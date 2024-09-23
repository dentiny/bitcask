package(default_visibility = ["//visibility:public"])

cc_library(
    name = "source_location",
    srcs = ["source_location.cc"],
    hdrs = ["source_location.h"],
    deps = [
        "@com_google_absl//absl/strings:str_format",
    ],
)

cc_test(
    name = "source_location_test",
    srcs = ["source_location_test.cc"],
    deps = [
        ":source_location",
        "//base:logging",
        "//utils/testing:test_main",
    ],
)

cc_library(
    name = "likely",
    hdrs = ["likely.h"],
    deps = [
        "@com_google_absl//absl/base:core_headers",
    ],
)
