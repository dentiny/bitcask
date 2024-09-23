# Install bazel 1.0.0
# Copied from https://bazel.build/install/ubuntu
sudo apt install apt-transport-https curl gnupg -y
curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --dearmor >bazel-archive-keyring.gpg
sudo mv bazel-archive-keyring.gpg /usr/share/keyrings
echo "deb [arch=amd64 signed-by=/usr/share/keyrings/bazel-archive-keyring.gpg] https://storage.googleapis.com/bazel-apt stable jdk1.8" | sudo tee /etc/apt/sources.list.d/bazel.list

sudo apt update -y && sudo apt full-upgrade -y
sudo apt install -y bazel-5.4.0
sudo ln -s /usr/bin/bazel-5.4.0 /usr/bin/bazel
sudo cp misc/bazel /etc/bash_completion.d
source /etc/bash_completion.d/bazel

# Install common tools, gdb, dstat, htop and perf
sudo apt install -y gdb dstat htop screen linux-tools-common linux-tools-generic linux-tools-`uname -r`

# Configure vim
sudo cp misc/vimrc.local /etc/vim/vimrc.local

# Configure git

# Setup push hook 
sudo apt install -y clang-format cppcheck python3-pip
sudo pip install cpplint pre-commit
pre-commit install -t pre-push

# Setup git icdiff
pip3 install icdiff
git clone git@github.com:jeffkaufman/icdiff.git
sudo cp icdiff/git-icdiff /usr/local/bin
rm -rf icdiff

# Default to use clang
sudo apt install -y clang
echo -e "# Defaults to use clang for compilation." >> ~/.bashrc
echo -e "export CC=clang\n" >> ~/.bashrc
source ~/.bashrc

# Install fzf
git clone --depth 1 https://github.com/junegunn/fzf.git ~/.fzf
~/.fzf/install
source ~/.bashrc

# Install llvm-symbolizer
sudo apt install -y unar
cd ~
wget https://github.com/llvm/llvm-project/releases/download/llvmorg-12.0.0/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-16.04.tar.xz
unar clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-16.04.tar.xz
cd ~/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-16.04/bin
sudo cp llvm-symbolizer /usr/bin/
which llvm-symbolizer
rm -rf clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-16.04.tar.xz
cd -

# Install fuck
pip3 install thefuck --user
echo 'eval "$(thefuck --alias)"' >> ~/.bashrc
source ~/.bashrc
