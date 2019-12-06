#!/usr/bin/env bash

cd .. && \
cloc *.* \
App/*.* \
App/modules/log/*.* \
App/modules/PacketProcessor/*.*
