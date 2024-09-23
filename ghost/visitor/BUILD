package(default_visibility = ["//visibility:public"])

cc_library(
    name = "visitor",
    hdrs = ["visitor.h"],
    deps = [
        "//base:logging",
        "//ghost/visitor/internal:visitor",
    ],
)

cc_test(
    name = "visitor_test",
    srcs = ["visitor_test.cc"],
    deps = [
        ":visitor",
        "//utils/strings:container_printer",
        "//utils/strings:std_byte_printer",
        "//utils/strings:std_printer",
        "//utils/strings:type_printer",
        "//utils/testing:test_main",
    ],
)
