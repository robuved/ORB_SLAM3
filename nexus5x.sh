#!/bin/bash
pathDatasetEuroc='/Datasets/EuRoC' #Example, it is necesary to change it by the dataset path

#------------------------------------
# Monocular Examples
echo "Launching owwn dataset with Monocular-Inertial sensors"
# gdb -args ./Examples/Monocular-Inertial/mono_inertial_euroc ./Vocabulary/ORBvoc.txt ./Examples/Monocular-Inertial/nexus5x.yaml /home/victor/Documents/MASTER_CERCETARE_nonessentials/output/real/in_place_8_motionstracking_True_30/37/asl /home/victor/Documents/MASTER_CERCETARE_nonessentials/output/real/in_place_8_motionstracking_True_30/37/asl/image_timestamps.txt /home/victor/orbslam3_ws_test/37/
./Examples/Monocular-Inertial/mono_inertial_nexus5x ./Vocabulary/ORBvoc.txt ./Examples/Monocular-Inertial/nexus5x.yaml /home/victor/Documents/MASTER_CERCETARE_nonessentials/output/real/in_place_8_motionstracking_True_30/37/asl /home/victor/Documents/MASTER_CERCETARE_nonessentials/output/real/in_place_8_motionstracking_True_30/37/asl/image_timestamps.txt /home/victor/orbslam3_ws_test/37
