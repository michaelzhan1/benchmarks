sudo -v
for tile in 1 2 4 8 16 32 64 128 256 512 1024 2048; do
  printf '\n=== %sx%s ===\n' "$tile" "$tile"
  sudo mperf-stat \
    -e cycles -e instructions \
    -e ARM_L1D_CACHE -e ARM_L1D_CACHE_REFILL \
    -e L1D_CACHE_MISS_LD -e L1D_CACHE_MISS_ST \
    -e L1D_CACHE_WRITEBACK \
    -e L1D_TLB_MISS -e L2_TLB_MISS_DATA \
    -- ./bin/transpose "$tile" 10
done