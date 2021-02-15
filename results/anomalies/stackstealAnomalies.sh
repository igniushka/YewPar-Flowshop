#!/bin/bash
mpiexec -disable-hostname-propagation -n 16 --hosts gpgnode-05,gpgnode-06,gpgnode-07,gpgnode-08,gpgnode-09,gpgnode-10,gpgnode-11,gpgnode-12,gpgnode-13,gpgnode-14,gpgnode-15,gpgnode-16,gpgnode-17,gpgnode-18,gpgnode-19,gpgnode-20 ./flowshop -f ../../../FSPinstances/ta22 --skel stacksteal --hpx:threads 16 >> ./anom/1/ta22-steal-16x16
sleep 30s
mpiexec -disable-hostname-propagation -n 16 --hosts gpgnode-05,gpgnode-06,gpgnode-07,gpgnode-08,gpgnode-09,gpgnode-10,gpgnode-11,gpgnode-12,gpgnode-13,gpgnode-14,gpgnode-15,gpgnode-16,gpgnode-17,gpgnode-18,gpgnode-19,gpgnode-20 ./flowshop -f ../../../FSPinstances/ta22 --skel stacksteal --hpx:threads 16 >> ./anom/2/ta22-steal-16x16
sleep 30s
mpiexec -disable-hostname-propagation -n 16 --hosts gpgnode-05,gpgnode-06,gpgnode-07,gpgnode-08,gpgnode-09,gpgnode-10,gpgnode-11,gpgnode-12,gpgnode-13,gpgnode-14,gpgnode-15,gpgnode-16,gpgnode-17,gpgnode-18,gpgnode-19,gpgnode-20 ./flowshop -f ../../../FSPinstances/ta22 --skel stacksteal --hpx:threads 16 >> ./anom/3/ta22-steal-16x16
sleep 30s
mpiexec -disable-hostname-propagation -n 16 --hosts gpgnode-05,gpgnode-06,gpgnode-07,gpgnode-08,gpgnode-09,gpgnode-10,gpgnode-11,gpgnode-12,gpgnode-13,gpgnode-14,gpgnode-15,gpgnode-16,gpgnode-17,gpgnode-18,gpgnode-19,gpgnode-20 ./flowshop -f ../../../FSPinstances/ta22 --skel stacksteal --hpx:threads 16 >> ./anom/4/ta22-steal-16x16
sleep 30s
mpiexec -disable-hostname-propagation -n 16 --hosts gpgnode-05,gpgnode-06,gpgnode-07,gpgnode-08,gpgnode-09,gpgnode-10,gpgnode-11,gpgnode-12,gpgnode-13,gpgnode-14,gpgnode-15,gpgnode-16,gpgnode-17,gpgnode-18,gpgnode-19,gpgnode-20 ./flowshop -f ../../../FSPinstances/ta22 --skel stacksteal --hpx:threads 16 >> ./anom/5/ta22-steal-16x16
sleep 30s
mpiexec -disable-hostname-propagation -n 16 --hosts gpgnode-05,gpgnode-06,gpgnode-07,gpgnode-08,gpgnode-09,gpgnode-10,gpgnode-11,gpgnode-12,gpgnode-13,gpgnode-14,gpgnode-15,gpgnode-16,gpgnode-17,gpgnode-18,gpgnode-19,gpgnode-20 ./flowshop -f ../../../FSPinstances/ta29 --skel stacksteal --hpx:threads 16 >> ./anom/1/ta29-steal-16x16
sleep 30s
mpiexec -disable-hostname-propagation -n 16 --hosts gpgnode-05,gpgnode-06,gpgnode-07,gpgnode-08,gpgnode-09,gpgnode-10,gpgnode-11,gpgnode-12,gpgnode-13,gpgnode-14,gpgnode-15,gpgnode-16,gpgnode-17,gpgnode-18,gpgnode-19,gpgnode-20 ./flowshop -f ../../../FSPinstances/ta29 --skel stacksteal --hpx:threads 16 >> ./anom/2/ta29-steal-16x16
sleep 30s
mpiexec -disable-hostname-propagation -n 16 --hosts gpgnode-05,gpgnode-06,gpgnode-07,gpgnode-08,gpgnode-09,gpgnode-10,gpgnode-11,gpgnode-12,gpgnode-13,gpgnode-14,gpgnode-15,gpgnode-16,gpgnode-17,gpgnode-18,gpgnode-19,gpgnode-20 ./flowshop -f ../../../FSPinstances/ta29 --skel stacksteal --hpx:threads 16 >> ./anom/3/ta29-steal-16x16
sleep 30s
mpiexec -disable-hostname-propagation -n 16 --hosts gpgnode-05,gpgnode-06,gpgnode-07,gpgnode-08,gpgnode-09,gpgnode-10,gpgnode-11,gpgnode-12,gpgnode-13,gpgnode-14,gpgnode-15,gpgnode-16,gpgnode-17,gpgnode-18,gpgnode-19,gpgnode-20 ./flowshop -f ../../../FSPinstances/ta29 --skel stacksteal --hpx:threads 16 >> ./anom/4/ta29-steal-16x16
sleep 30s
mpiexec -disable-hostname-propagation -n 16 --hosts gpgnode-05,gpgnode-06,gpgnode-07,gpgnode-08,gpgnode-09,gpgnode-10,gpgnode-11,gpgnode-12,gpgnode-13,gpgnode-14,gpgnode-15,gpgnode-16,gpgnode-17,gpgnode-18,gpgnode-19,gpgnode-20 ./flowshop -f ../../../FSPinstances/ta29 --skel stacksteal --hpx:threads 16 >> ./anom/5/ta29-steal-16x16
sleep 30s