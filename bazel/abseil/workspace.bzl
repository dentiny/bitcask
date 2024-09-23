load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def com_github_abseil():
    http_archive(
        name = "com_google_absl",
        patch_args = ["-p1"],
        patches = [
            "//bazel/abseil:remove_wasm.patch",
        ],
        sha256 = "59d2976af9d6ecf001a81a35749a6e551a335b949d34918cfade07737b9d93c5",
        strip_prefix = "abseil-cpp-20230802.0",
        urls = [
            "https://github.com/abseil/abseil-cpp/archive/refs/tags/20230802.0.tar.gz",
        ],
    )
