package(default_visibility = ["//visibility:public"])

cc_library(
    name = "scope_guard",
    srcs = ["scope_guard.h"],
    deps = [
        "//base:meta",
    ],
)

cc_test(
    name = "scope_guard_test",
    srcs = ["scope_guard_test.cc"],
    deps = [
        ":scope_guard",
        "//utils/testing:test_main",
    ],
)

cc_library(
    name = "scoped_directory",
    srcs = ["scoped_directory.cc"],
    hdrs = ["scoped_directory.h"],
    deps = [
        "//base/file",
        "//utils/status",
    ],
)

cc_test(
    name = "scoped_directory_test",
    srcs = ["scoped_directory_test.cc"],
    deps = [
        ":scoped_directory",
        "//base:logging",
        "//utils/testing:test_main",
    ],
)

cc_library(
    name = "time_utils",
    hdrs = ["time_utils.h"],
)

cc_library(
    name = "thread_pool",
    srcs = ["thread_pool.cc"],
    hdrs = ["thread_pool.h"],
    deps = [
        "//ghost/mutex",
        "//utils/status",
    ],
)
