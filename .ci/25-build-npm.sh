#!/usr/bin/env bash

# For manual use

# Get the root directory of the repo

repo_root=$(git rev-parse --show-toplevel)

# copy package and define version
version=$(bash "${repo_root}"/.ci/js-version.bash)
echo "$version"
cd "${repo_root}"/.ci/nrp-jsproto/ || exit 1
# todo: automatize somehow the path definition
cp "${repo_root}"/build/nrp_protobuf/js-dist/nrp-jsproto/nrp-jsproto.js ./
sed -i -E "s/\"version\":\s+\".*\"/\"version\": \"${version}\"/" package.json

# install npm
sudo apt update && sudo apt install -y nodejs npm

npm login
npm publish
npm logout

# EOF
