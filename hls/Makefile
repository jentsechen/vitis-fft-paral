PLATFORM=/media/jtc/Data/2024.2/Vitis/2024.2/base_platforms/xilinx_vck190_base_202420_1/xilinx_vck190_base_202420_1.xpfm

mm2s = ./mm2s/mm2s.xo
s2mm = ./s2mm/s2mm.xo
all: $(mm2s) $(s2mm)

$(mm2s): mm2s.cpp mm2s.cfg
	v++ -c --mode hls --platform ${PLATFORM} --config ./mm2s.cfg

$(s2mm): s2mm.cpp s2mm.cfg
	v++ -c --mode hls --platform ${PLATFORM} --config ./s2mm.cfg

clean:
	rm -rf ./mm2s/ ./s2mm/