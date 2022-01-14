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

# NPM_USER, NPM_PASS, NPM_REGISTRY should be set
[ -z $NPM_USER ] && exit 1
[ -z $NPM_PASS ] && exit 1
[ -z $NPM_REGISTRY ] && exit 1
export NPM_RC_PATH="${repo_root}"/.ci/nrp-jsproto/.npmrc
export NPM_EMAIL=ci@neurorobotics.eu

# install npm
sudo apt update && sudo apt install -y nodejs npm
npm install -g npm-cli-login

rm -rf .npmrc
npm-cli-login
sed -i "s/.*://" .npmrc
echo "registry=${NPM_REGISTRY}" >> .npmrc

npm-cli-login
npm publish
npm logout --registry="${NPM_REGISTRY}"

# EOF
