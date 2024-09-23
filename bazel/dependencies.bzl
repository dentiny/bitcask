load(
    "@bazel_tools//tools/build_defs/repo:git.bzl",
    "git_repository",
)
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("//bazel/abseil:workspace.bzl", "com_github_abseil")
load("//bazel/glog:workspace.bzl", "com_glog_workspace")
load("//bazel/google_crc32:workspace.bzl", "google_crc32_workspace")

def ghost_dependencies():
    _com_github_google_googletest()
    _com_github_google_benchmark()
    _com_gflag_workspace()
    com_github_abseil()
    com_glog_workspace()
    google_crc32_workspace()
    magic_enum_workspace()

def _com_github_google_googletest():
    http_archive(
        name = "gtest",
        sha256 = "da0f704133e746bb248358ce05322ec9013c3c32449408e9235b4ebf23dbf6b6",
        strip_prefix = "googletest-ca4b7c9ff4d8a5c37ac51795b03ffe934958aeff",
        urls = ["https://github.com/google/googletest/archive/ca4b7c9ff4d8a5c37ac51795b03ffe934958aeff.tar.gz"],
    )

def _com_github_google_benchmark():
    git_repository(
        name = "benchmark",
        remote = "https://github.com/google/benchmark",
        tag = "v1.5.0",
    )

def magic_enum_workspace():
    http_archive(
        name = "magic_enum",
        sha256 = "fcda8295256a2084f1f98a63b3d2c66b3d7140eea008e1ef94ea015b2f6d3034",
        strip_prefix = "magic_enum-0.9.6/",
        urls = [
            "https://github.com/Neargye/magic_enum/archive/refs/tags/v0.9.6.tar.gz",
        ],
    )

def _com_gflag_workspace():
    http_archive(
        name = "com_github_gflags_gflags",
        sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
        strip_prefix = "gflags-2.2.2/",
        urls = ["https://github.com/gflags/gflags/archive/v2.2.2.tar.gz"],
    )
