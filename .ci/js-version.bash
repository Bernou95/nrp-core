#!/bin/bash

VERSION=$(git describe --abbrev=1 --tags)

#replace . with space so can split into an array
VERSION_BITS=(${VERSION//./ })

#get number parts and increase last one by 1 (future version), add pre-release version and build info
VNUM1=${VERSION_BITS[0]}
VNUM2=${VERSION_BITS[1]}
VNUM3LONG=${VERSION_BITS[2]}

VNUM3_BITS=(${VNUM3LONG//-/ })

VNUM3=${VNUM3_BITS[0]}
DIST=${VNUM3_BITS[1]}
HASH=${VNUM3_BITS[2]}

if [ ! -z "${DIST}" ]; then
    VNUM3=$((VNUM3+1))
    VERSION=$VNUM1.$VNUM2.$VNUM3-alpha.${DIST}+${HASH}
fi

echo "${VERSION}"
