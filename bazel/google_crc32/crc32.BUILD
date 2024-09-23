load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

filegroup(
    name = "all",
    srcs = glob(["**/*"]),
)

COMMON_OPTIONS = {
    "CRC32C_BUILD_TESTS": "OFF",
    "CRC32C_BUILD_BENCHMARKS": "OFF",
    "CRC32C_USE_GLOG": "OFF",
    "CRC32C_INSTALL": "ON",
    "BUILD_SHARED_LIBS": "OFF",
}

cmake(
    name = "libcrc32",
    cache_entries = select({
        "@//base/rules:compilation_mode_dbg": dict(COMMON_OPTIONS.items() + {
            "CMAKE_BUILD_TYPE": "Debug",
        }.items()),
        "//conditions:default": dict(COMMON_OPTIONS.items() + {
            "CMAKE_BUILD_TYPE": "Release",
        }.items()),
    }),
    env = {
        "CMAKE_BUILD_PARALLEL_LEVEL": "$$(nproc)",
    },
    features = [
        "-warnings_to_error",
    ],
    lib_source = ":all",
    out_static_libs = [
        "libcrc32c.a",
    ],
    visibility = ["//visibility:public"],
)
