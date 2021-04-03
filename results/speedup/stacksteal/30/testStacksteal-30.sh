#!/bin/bash
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta30 --skel stacksteal --hpx:threads 16 >> ./steal/30/1/ta30-steal-2x16
sleep 30s
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta29 --skel stacksteal --hpx:threads 16 >> ./steal/30/1/ta29-steal-2x16
sleep 30s
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta28 --skel stacksteal --hpx:threads 16 >> ./steal/30/1/ta28-steal-2x16
sleep 30s
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta27 --skel stacksteal --hpx:threads 16 >> ./steal/30/1/ta27-steal-2x16
sleep 30s
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta26 --skel stacksteal --hpx:threads 16 >> ./steal/30/1/ta26-steal-2x16
sleep 30s
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta25 --skel stacksteal --hpx:threads 16 >> ./steal/30/1/ta25-steal-2x16
sleep 30s
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta24 --skel stacksteal --hpx:threads 16 >> ./steal/30/1/ta24-steal-2x16
sleep 30s
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta23 --skel stacksteal --hpx:threads 16 >> ./steal/30/1/ta23-steal-2x16
sleep 30s
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta22 --skel stacksteal --hpx:threads 16 >> ./steal/30/1/ta22-steal-2x16
sleep 30s
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta21 --skel stacksteal --hpx:threads 16 >> ./steal/30/1/ta21-steal-2x16
sleep 30s
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta30 --skel stacksteal --hpx:threads 16 >> ./steal/30/2/ta30-steal-2x16
sleep 30s
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta29 --skel stacksteal --hpx:threads 16 >> ./steal/30/2/ta29-steal-2x16
sleep 30s
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta28 --skel stacksteal --hpx:threads 16 >> ./steal/30/2/ta28-steal-2x16
sleep 30s
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta27 --skel stacksteal --hpx:threads 16 >> ./steal/30/2/ta27-steal-2x16
sleep 30s
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta26 --skel stacksteal --hpx:threads 16 >> ./steal/30/2/ta26-steal-2x16
sleep 30s
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta25 --skel stacksteal --hpx:threads 16 >> ./steal/30/2/ta25-steal-2x16
sleep 30s
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta24 --skel stacksteal --hpx:threads 16 >> ./steal/30/2/ta24-steal-2x16
sleep 30s
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta23 --skel stacksteal --hpx:threads 16 >> ./steal/30/2/ta23-steal-2x16
sleep 30s
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta22 --skel stacksteal --hpx:threads 16 >> ./steal/30/2/ta22-steal-2x16
sleep 30s
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta21 --skel stacksteal --hpx:threads 16 >> ./steal/30/2/ta21-steal-2x16
sleep 30s
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta30 --skel stacksteal --hpx:threads 16 >> ./steal/30/3/ta30-steal-2x16
sleep 30s
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta29 --skel stacksteal --hpx:threads 16 >> ./steal/30/3/ta29-steal-2x16
sleep 30s
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta28 --skel stacksteal --hpx:threads 16 >> ./steal/30/3/ta28-steal-2x16
sleep 30s
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta27 --skel stacksteal --hpx:threads 16 >> ./steal/30/3/ta27-steal-2x16
sleep 30s
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta26 --skel stacksteal --hpx:threads 16 >> ./steal/30/3/ta26-steal-2x16
sleep 30s
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta25 --skel stacksteal --hpx:threads 16 >> ./steal/30/3/ta25-steal-2x16
sleep 30s
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta24 --skel stacksteal --hpx:threads 16 >> ./steal/30/3/ta24-steal-2x16
sleep 30s
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta23 --skel stacksteal --hpx:threads 16 >> ./steal/30/3/ta23-steal-2x16
sleep 30s
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta22 --skel stacksteal --hpx:threads 16 >> ./steal/30/3/ta22-steal-2x16
sleep 30s
mpiexec -disable-hostname-propagation -n 2 --hosts gpgnode-03,gpgnode-04 ./flowshop -f ../../../FSPinstances/ta21 --skel stacksteal --hpx:threads 16 >> ./steal/30/3/ta21-steal-2x16
sleep 30s

