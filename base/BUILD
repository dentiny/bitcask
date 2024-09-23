package(default_visibility = ["//visibility:public"])

cc_library(
    name = "init_ghost",
    srcs = ["init_ghost.cc"],
    hdrs = ["init_ghost.h"],
    deps = [
        "@com_github_gflags_gflags//:gflags",
        "@com_google_glog//:glog",
    ],
)

cc_library(
    name = "logging",
    hdrs = ["logging.h"],
    deps = [
        "@com_google_glog//:glog",
    ],
)

cc_library(
    name = "meta",
    hdrs = ["meta.h"],
)

cc_test(
    name = "meta_test",
    srcs = ["meta_test.cc"],
    deps = [
        ":meta",
        "//utils/testing:test_main",
    ],
)

cc_library(
    name = "type_traits",
    hdrs = ["type_traits.h"],
    deps = [
        "@com_google_absl//absl/strings:str_format",
    ],
)

cc_test(
    name = "type_traits_test",
    srcs = ["type_traits_test.cc"],
    deps = [
        ":type_traits",
        "//utils/testing:test_main",
        "@com_google_absl//absl/container:flat_hash_map",
    ],
)

cc_library(
    name = "memory_view",
    hdrs = ["memory_view.h"],
    deps = [
        "//utils/status",
        "@com_google_absl//absl/base:nullability",
    ],
)

cc_test(
    name = "memory_view_test",
    srcs = ["memory_view_test.cc"],
    deps = [
        ":memory_view",
        "//utils/testing:test_main",
    ],
)
