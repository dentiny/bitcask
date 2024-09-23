package(default_visibility = ["//visibility:public"])

cc_library(
    name = "status",
    srcs = [
        "status.cc",
        "status_code.cc",
    ],
    hdrs = [
        "status.h",
        "status_code.h",
        "status_exception.h",
        "status_macros.h",
        "status_or.h",
    ],
    deps = [
        "//base:logging",
        "//base:meta",
        "//base:type_traits",
        "//ghost:likely",
        "//ghost:source_location",
        "//utils/container:static_map",
        "//utils/strings:debug_printer",
        "@com_google_absl//absl/base:core_headers",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/container:inlined_vector",
        "@com_google_absl//absl/strings",
    ],
)

cc_test(
    name = "status_test",
    srcs = ["status_test.cc"],
    deps = [
        ":status",
        "//base:logging",
        "//utils/testing:test_main",
    ],
)

cc_test(
    name = "status_or_test",
    srcs = ["status_or_test.cc"],
    deps = [
        ":status",
        ":testing",
        "//utils/testing:test_main",
    ],
)

cc_test(
    name = "status_macros_test",
    srcs = ["status_macros_test.cc"],
    deps = [
        ":status",
        ":testing",
        "//base:logging",
        "//ghost:source_location",
        "//utils/testing:test_main",
        "@com_google_absl//absl/container:flat_hash_map",
    ],
)

cc_library(
    name = "compat_errno",
    hdrs = ["compat_errno.h"],
    deps = [
        ":status",
    ],
)

cc_library(
    name = "compat_errno_macros",
    hdrs = ["compat_errno_macros.h"],
    deps = [
        ":compat_errno",
        ":status",
        "//ghost:likely",
        "//ghost:source_location",
    ],
)

cc_test(
    name = "compat_errno_macros_test",
    srcs = ["compat_errno_macros_test.cc"],
    deps = [
        ":compat_errno_macros",
        ":testing",
        "//utils/testing:test_main",
    ],
)

cc_library(
    name = "testing",
    hdrs = ["testing.h"],
    deps = [
        ":status",
        "//base:meta",
        "@gtest",
    ],
)

cc_test(
    name = "testing_test",
    srcs = ["testing_test.cc"],
    deps = [
        ":status",
        ":testing",
        "//utils/testing:test_main",
    ],
)
