load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def google_crc32_workspace():
    http_archive(
        name = "google_crc32",
        build_file = "//bazel/google_crc32:crc32.BUILD",
        sha256 = "ac07840513072b7fcebda6e821068aa04889018f24e10e46181068fb214d7e56",
        strip_prefix = "crc32c-1.1.2",
        url = "https://github.com/google/crc32c/archive/refs/tags/1.1.2.tar.gz",
    )
