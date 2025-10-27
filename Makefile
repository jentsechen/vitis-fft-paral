TARGET=hw

PLATFORM_REPO_PATHS=/media/jtc/Data/2024.2/Vitis/2024.2/base_platforms/xilinx_vck190_base_202420_1/
PLATFORM=/media/jtc/Data/2024.2/Vitis/2024.2/base_platforms/xilinx_vck190_base_202420_1/xilinx_vck190_base_202420_1.xpfm
COMMON_IMAGE_VERSAL=/media/jtc/Data/2024.2/xilinx-versal-common-v2024.2
export ROOTFS=${COMMON_IMAGE_VERSAL}/rootfs.ext4
export IMAGE=${COMMON_IMAGE_VERSAL}/Image
export CXX=${XILINX_VITIS}/gnu/aarch64/lin/aarch64-linux/bin/aarch64-linux-gnu-g++
export SDKTARGETSYSROOT=${COMMON_IMAGE_VERSAL}/sysroots/cortexa72-cortexa53-xilinx-linux

XSA=graph_${TARGET}.xsa
HOST_EXE=host.exe

GRAPH=aie/graph.cpp
LIBADF=libadf.a
MM2S = ./hls/mm2s/mm2s.xo
S2MM = ./hls/s2mm/s2mm.xo
HOST=host.exe

SYS_CFG=system.cfg
VPP_FLAGS=--save-temps --verbose --config ${SYS_CFG}  

aie: ${LIBADF}
# ${LIBADF}: aie/*
# 	v++ -c --mode aie --platform=${PLATFORM} \
# 		-I../Vitis_Libraries/dsp/L1/include/aie \
# 		-I../Vitis_Libraries/dsp/L1/src/aie \
# 		-I../Vitis_Libraries/dsp/L2/include/aie \
# 		--include="./aie" --work_dir=./Work \
# 		${GRAPH} 2>&1 | tee log.txt
${LIBADF}: aie/*
	v++ -c --mode aie --platform=${PLATFORM} \
		-I../Vitis_Libraries/dsp/L1/include/aie \
		-I../Vitis_Libraries/dsp/L1/src/aie \
		-I../Vitis_Libraries/dsp/L2/include/aie \
		--include="./aie" --work_dir=./Work \
		--aie.constraints="aie_constraints.json" \
		${GRAPH} 2>&1 | tee log.txt

xsa: ${XSA}
host: ${HOST}
package: package_${TARGET}

hls: hls/*
	make all -C hls

${XSA}: ${MM2S} ${S2MM} ${LIBADF} ${SYS_CFG} 
	v++ -g -l --platform ${PLATFORM} ${MM2S} ${S2MM} ${LIBADF} -t ${TARGET} ${VPP_FLAGS} -o $@

${HOST}: ${GRAPH} ./Work/ps/c_rts/aie_control_xrt.cpp ./host/host.cpp
	$(MAKE) -C host/

package_${TARGET}: ${LIBADF} ${XSA} ${HOST} 
	v++ -p -t ${TARGET} -f ${PLATFORM} \
		--package.rootfs ${ROOTFS} \
		--package.kernel_image ${IMAGE} \
		--package.boot_mode=sd \
		--package.image_format=ext4 \
		--package.defer_aie_run \
		--package.sd_file ${HOST} ${XSA} ${LIBADF}

clean:
	rm -rf _x v++_* ${XOS} ${OS} ${LIBADF} *.o.* *.o *.xpe *.xo.* \
	       *.xclbin* *.xsa *.log *.jou xnwOut Work Map_Report.csv \
	       ilpProblem* sol.db drivers .Xil  aiesimu* x86simu*