#!/bin/bash
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta30 --skel stacksteal --hpx:threads 16 >>  ./steal/60/1/ta30-steal-4x16
sleep 30s
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta29 --skel stacksteal --hpx:threads 16 >>  ./steal/60/1/ta29-steal-4x16
sleep 30s
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta28 --skel stacksteal --hpx:threads 16 >>  ./steal/60/1/ta28-steal-4x16
sleep 30s
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta27 --skel stacksteal --hpx:threads 16 >>  ./steal/60/1/ta27-steal-4x16
sleep 30s
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta26 --skel stacksteal --hpx:threads 16 >>  ./steal/60/1/ta26-steal-4x16
sleep 30s
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta25 --skel stacksteal --hpx:threads 16 >>  ./steal/60/1/ta25-steal-4x16
sleep 30s
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta24 --skel stacksteal --hpx:threads 16 >>  ./steal/60/1/ta24-steal-4x16
sleep 30s
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta23 --skel stacksteal --hpx:threads 16 >>  ./steal/60/1/ta23-steal-4x16
sleep 30s
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta22 --skel stacksteal --hpx:threads 16 >>  ./steal/60/1/ta22-steal-4x16
sleep 30s
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta21 --skel stacksteal --hpx:threads 16 >>  ./steal/60/1/ta21-steal-4x16
sleep 30s
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta30 --skel stacksteal --hpx:threads 16 >>  ./steal/60/2/ta30-steal-4x16
sleep 30s
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta29 --skel stacksteal --hpx:threads 16 >>  ./steal/60/2/ta29-steal-4x16
sleep 30s
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta28 --skel stacksteal --hpx:threads 16 >>  ./steal/60/2/ta28-steal-4x16
sleep 30s
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta27 --skel stacksteal --hpx:threads 16 >>  ./steal/60/2/ta27-steal-4x16
sleep 30s
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta26 --skel stacksteal --hpx:threads 16 >>  ./steal/60/2/ta26-steal-4x16
sleep 30s
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta25 --skel stacksteal --hpx:threads 16 >>  ./steal/60/2/ta25-steal-4x16
sleep 30s
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta24 --skel stacksteal --hpx:threads 16 >>  ./steal/60/2/ta24-steal-4x16
sleep 30s
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta23 --skel stacksteal --hpx:threads 16 >>  ./steal/60/2/ta23-steal-4x16
sleep 30s
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta22 --skel stacksteal --hpx:threads 16 >>  ./steal/60/2/ta22-steal-4x16
sleep 30s
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta21 --skel stacksteal --hpx:threads 16 >>  ./steal/60/2/ta21-steal-4x16
sleep 30s
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta30 --skel stacksteal --hpx:threads 16 >>  ./steal/60/3/ta30-steal-4x16
sleep 30s
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta29 --skel stacksteal --hpx:threads 16 >>  ./steal/60/3/ta29-steal-4x16
sleep 30s
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta28 --skel stacksteal --hpx:threads 16 >>  ./steal/60/3/ta28-steal-4x16
sleep 30s
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta27 --skel stacksteal --hpx:threads 16 >>  ./steal/60/3/ta27-steal-4x16
sleep 30s
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta26 --skel stacksteal --hpx:threads 16 >>  ./steal/60/3/ta26-steal-4x16
sleep 30s
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta25 --skel stacksteal --hpx:threads 16 >>  ./steal/60/3/ta25-steal-4x16
sleep 30s
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta24 --skel stacksteal --hpx:threads 16 >>  ./steal/60/3/ta24-steal-4x16
sleep 30s
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta23 --skel stacksteal --hpx:threads 16 >>  ./steal/60/3/ta23-steal-4x16
sleep 30s
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta22 --skel stacksteal --hpx:threads 16 >>  ./steal/60/3/ta22-steal-4x16
sleep 30s
mpiexec -disable-hostname-propagation -n 4 --hosts  gpgnode-12,gpgnode-11,gpgnode-10,gpgnode-09 ./flowshop -f ../../../FSPinstances/ta21 --skel stacksteal --hpx:threads 16 >>  ./steal/60/3/ta21-steal-4x16
sleep 30s
