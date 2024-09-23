package(default_visibility = ["//visibility:public"])

cc_library(
    name = "file",
    srcs = ["file.cc"],
    hdrs = ["file.h"],
    deps = [
        ":base_file_handle",
        ":os_read_handle",
        ":os_rw_handle",
        ":os_write_handle",
        "//utils/status",
        "//utils/status:compat_errno_macros",
    ],
)

cc_library(
    name = "base_file_handle",
    hdrs = ["base_file_handle.h"],
    deps = [
        "//base:memory_view",
        "//utils/status",
        "@com_google_absl//absl/base:nullability",
    ],
)

cc_library(
    name = "base_read_handle",
    hdrs = ["base_read_handle.h"],
    deps = [
        ":base_file_handle",
        "//base:memory_view",
        "//utils/status",
        "@com_google_absl//absl/base:nullability",
    ],
)

cc_library(
    name = "mock_file_handle",
    hdrs = ["mock_file_handle.h"],
    deps = [
        ":base_read_handle",
        ":base_write_handle",
        "@gtest",
    ],
)

cc_library(
    name = "os_read_handle",
    srcs = ["os_read_handle.cc"],
    hdrs = ["os_read_handle.h"],
    deps = [
        ":base_read_handle",
        "//utils/status",
        "//utils/status:compat_errno_macros",
        "//utils/strings:resize_uninitialized",
        "@com_google_absl//absl/memory",
    ],
)

cc_test(
    name = "os_read_handle_test",
    srcs = ["os_read_handle_test.cc"],
    data = ["//base/file/testdata"],
    deps = [
        ":os_read_handle",
        "//utils/status:testing",
        "//utils/testing:test_main",
    ],
)

cc_library(
    name = "base_write_handle",
    hdrs = ["base_write_handle.h"],
    deps = [
        ":base_file_handle",
        "//base:memory_view",
        "//utils/status",
        "@com_google_absl//absl/base:nullability",
    ],
)

cc_library(
    name = "os_write_handle",
    srcs = ["os_write_handle.cc"],
    hdrs = ["os_write_handle.h"],
    deps = [
        ":base_write_handle",
        "//base:logging",
        "//utils/status",
        "//utils/status:compat_errno_macros",
        "@com_google_absl//absl/memory",
    ],
)

cc_test(
    name = "os_write_handle_test",
    srcs = ["os_write_handle_test.cc"],
    deps = [
        ":file",
        ":os_read_handle",
        ":os_write_handle",
        "//utils:scope_guard",
        "//utils/status:testing",
        "//utils/testing:test_main",
    ],
)

cc_library(
    name = "os_rw_handle",
    srcs = ["os_rw_handle.cc"],
    hdrs = ["os_rw_handle.h"],
    deps = [
        ":base_write_handle",
        "//base:logging",
        "//utils/status",
        "//utils/status:compat_errno_macros",
        "//utils/strings:resize_uninitialized",
        "@com_google_absl//absl/memory",
    ],
)
