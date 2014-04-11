#!/bin/sh
if [ -L /rr/current/.shared/default/etc/alternatives/cray-nvidia ]
then
VERNO=$(ls -l /rr/current/.shared/default/etc/alternatives/cray-nvidia |awk -F\/ '{print $12}')
BASE=/rr/current

cd ${BASE}/usr/local/X11R7.7/lib
for i in `ls ${BASE}/opt/cray/nvidia/$VERNO/lib64/lib*|awk -F\/ '{print $9}'`;do ln -sf /dsl/opt/cray/nvidia/$VERNO/lib64/$i $i;done;

cd ${BASE}/usr/local/X11R7.7/lib/tls
for i in `ls ${BASE}/opt/cray/nvidia/$VERNO/lib64/tls/lib*|awk -F\/ '{print $10}'`;do ln -sf /dsl/opt/cray/nvidia/$VERNO/lib64/tls/$i $i;ln -sf /dsl/opt/cray/nvidia/$VERNO/lib64/tls/$i ../$i;done;

cd ${BASE}/usr/local/X11R7.7/lib/xorg/modules

for i in `ls ${BASE}/opt/cray/nvidia/${VERNO}/lib64/lib*wfb*|awk -F\/ '{print $9}'`;do ln -sf /dsl/opt/cray/nvidia/$VERNO/lib64/$i $i;done;

cd ${BASE}/usr/local/X11R7.7/lib/xorg/modules/extensions
for i in `ls ${BASE}/opt/cray/nvidia/${VERNO}/lib64/libglx*|awk -F\/ '{print $9}'`;do ln -sf /dsl/opt/cray/nvidia/$VERNO/lib64/$i $i;ln -sf $i libglx.so;done;

cd ${BASE}/usr/local/X11R7.7/lib/xorg/modules/drivers

for i in `ls ${BASE}/opt/cray/nvidia/${VERNO}/lib64/nvidia_d*|awk -F\/ '{print $9}'`;do ln -sf /dsl/opt/cray/nvidia/$VERNO/lib64/$i $i;done;

else
VERNO=$(ls -l /etc/alternatives/cray-nvidia|awk -F\/ '{print $8}')
BASE=

cd ${BASE}/usr/local/X11R7.7/lib
for i in `ls ${BASE}/opt/cray/nvidia/$VERNO/lib64/lib*|awk -F\/ '{print $7}'`;do ln -sf /dsl/opt/cray/nvidia/$VERNO/lib64/$i $i;done;

cd ${BASE}/usr/local/X11R7.7/lib/tls
for i in `ls ${BASE}/opt/cray/nvidia/$VERNO/lib64/tls/lib*|awk -F\/ '{print $8}'`;do ln -sf /dsl/opt/cray/nvidia/$VERNO/lib64/tls/$i $i;ln -sf /dsl/opt/cray/nvidia/$VERNO/lib64/tls/$i ../$i;done;

cd ${BASE}/usr/local/X11R7.7/lib/xorg/modules

for i in `ls ${BASE}/opt/cray/nvidia/${VERNO}/lib64/lib*wfb*|awk -F\/ '{print $7}'`;do ln -sf /dsl/opt/cray/nvidia/$VERNO/lib64/$i $i;done;

cd ${BASE}/usr/local/X11R7.7/lib/xorg/modules/extensions
for i in `ls ${BASE}/opt/cray/nvidia/${VERNO}/lib64/libglx*|awk -F\/ '{print $7}'`;do ln -sf /dsl/opt/cray/nvidia/$VERNO/lib64/$i $i;ln -sf $i libglx.so;done;

cd ${BASE}/usr/local/X11R7.7/lib/xorg/modules/drivers

for i in `ls ${BASE}/opt/cray/nvidia/${VERNO}/lib64/nvidia_d*|awk -F\/ '{print $7}'`;do ln -sf /dsl/opt/cray/nvidia/$VERNO/lib64/$i $i;done;

fi


