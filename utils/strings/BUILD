package(default_visibility = ["//visibility:public"])

cc_library(
    name = "resize_uninitialized",
    hdrs = ["resize_uninitialized.h"],
    deps = [
        "@com_google_absl//absl/base:nullability",
        "@com_google_absl//absl/strings:internal",
    ],
)

cc_test(
    name = "resize_uninitialized_test",
    srcs = ["resize_uninitialized_test.cc"],
    deps = [
        ":resize_uninitialized",
        "//utils/testing:test_main",
    ],
)

cc_library(
    name = "type_printer",
    hdrs = ["type_printer.h"],
    deps = [
        ":get_type_name",
        "@boost//:core",
    ],
)

cc_library(
    name = "std_printer",
    hdrs = ["std_printer.h"],
    deps = [
        ":ostream_printer",
        "//base:logging",
    ],
)

cc_library(
    name = "absl_stringify_printer",
    hdrs = ["absl_stringify_printer.h"],
    deps = [
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "container_printer",
    hdrs = ["container_printer.h"],
    deps = [
        ":ostream_printer",
        "//base:logging",
        "//base:type_traits",
    ],
)

cc_library(
    name = "ostream_printer",
    hdrs = ["ostream_printer.h"],
    deps = [
        "//base:logging",
    ],
)

cc_test(
    name = "ostream_printer_test",
    srcs = ["ostream_printer_test.cc"],
    deps = [
        ":ostream_printer",
        "//utils/testing:test_main",
    ],
)

cc_library(
    name = "enum_printer",
    hdrs = ["enum_printer.h"],
    deps = [
        "@magic_enum",
    ],
)

cc_library(
    name = "debug_printer",
    hdrs = ["debug_printer.h"],
    deps = [
        "//ghost/visitor",
        "//utils/strings:absl_stringify_printer",
        "//utils/strings:container_printer",
        "//utils/strings:enum_printer",
        "//utils/strings:ostream_printer",
        "//utils/strings:std_printer",
        "//utils/strings:type_printer",
    ],
)

cc_test(
    name = "debug_printer_test",
    srcs = ["debug_printer_test.cc"],
    deps = [
        ":debug_printer",
        "//utils/testing:test_main",
    ],
)

cc_library(
    name = "get_type_name",
    hdrs = ["get_type_name.h"],
)

cc_test(
    name = "get_type_name_test",
    srcs = ["get_type_name_test.cc"],
    deps = [
        ":get_type_name",
        "//utils/testing:test_main",
    ],
)
