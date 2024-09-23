package(default_visibility = ["//visibility:public"])

filegroup(
    name = "testdata",
    srcs = glob(["**/*"]),
)

proto_library(
    name = "test_proto",
    srcs = ["test_proto.proto"],
)

cc_proto_library(
    name = "test_cc_proto",
    deps = [":test_proto"],
)
