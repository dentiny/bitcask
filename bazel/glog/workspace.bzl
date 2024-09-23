load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def com_glog_workspace():
    http_archive(
        name = "com_google_glog",
        sha256 = "8a83bf982f37bb70825df71a9709fa90ea9f4447fb3c099e1d720a439d88bad6",
        strip_prefix = "glog-0.6.0/",
        urls = ["https://github.com/google/glog/archive/v0.6.0.tar.gz"],
    )
