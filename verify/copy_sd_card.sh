#!/bin/bash
USER="petalinux"
HOST="10.100.70.13"
PASS="petalinux"
REMOTE_PATH="."
LOCAL_PATH="/media/jtc/Data/2024.2/Vitis/vitis-fft-paral/sd_card"
DATA_FILE_PATH="/media/jtc/Data/2024.2/Vitis/vitis-fft-paral/verify/data_rx_10_complex_64.npy"

ssh-keygen -R "$HOST" >/dev/null || true
sshpass -p "$PASS" sftp -o StrictHostKeyChecking=accept-new ${USER}@${HOST} <<EOF
cd ${REMOTE_PATH}
lcd ${LOCAL_PATH}
mput *
put ${DATA_FILE_PATH}
bye
EOF
