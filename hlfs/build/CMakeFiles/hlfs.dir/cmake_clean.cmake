FILE(REMOVE_RECURSE
  "CMakeFiles/hlfs.dir/storage/hlfs_write.c.o"
  "CMakeFiles/hlfs.dir/storage/init_hlfs.c.o"
  "CMakeFiles/hlfs.dir/storage/seg_clean_task.c.o"
  "CMakeFiles/hlfs.dir/storage/hlfs_read.c.o"
  "CMakeFiles/hlfs.dir/storage/hlfs_stat.c.o"
  "CMakeFiles/hlfs.dir/storage/hlfs_ctrl.c.o"
  "CMakeFiles/hlfs.dir/storage/hlfs_open.c.o"
  "CMakeFiles/hlfs.dir/storage/hlfs_close.c.o"
  "CMakeFiles/hlfs.dir/storage/deinit_hlfs.c.o"
  "CMakeFiles/hlfs.dir/logger/segfile_handler_optmize.c.o"
  "CMakeFiles/hlfs.dir/logger/log_ops.c.o"
  "CMakeFiles/hlfs.dir/logger/block_ops.c.o"
  "CMakeFiles/hlfs.dir/backend/local_storage.c.o"
  "CMakeFiles/hlfs.dir/backend/hdfs_storage.c.o"
  "CMakeFiles/hlfs.dir/clean/seg_clean_helper.c.o"
  "CMakeFiles/hlfs.dir/clean/migrate_alive_blocks.c.o"
  "CMakeFiles/hlfs.dir/clean/load_seg_usage_by_order.c.o"
  "CMakeFiles/hlfs.dir/clean/seg_usage_calc.c.o"
  "CMakeFiles/hlfs.dir/clean/seg_usage_dump.c.o"
  "CMakeFiles/hlfs.dir/utils/storage_helper.c.o"
  "CMakeFiles/hlfs.dir/utils/misc.c.o"
  "CMakeFiles/hlfs.dir/utils/address.c.o"
  "CMakeFiles/hlfs.dir/snapshot/snapshot_helper.c.o"
  "CMakeFiles/hlfs.dir/snapshot/hlfs_find_inode_before_time.c.o"
  "CMakeFiles/hlfs.dir/snapshot/hlfs_list_all_snapshots.c.o"
  "CMakeFiles/hlfs.dir/snapshot/hlfs_get_inode_info.c.o"
  "CMakeFiles/hlfs.dir/snapshot/hlfs_open_by_snapshot.c.o"
  "CMakeFiles/hlfs.dir/snapshot/hlfs_open_by_inode.c.o"
  "CMakeFiles/hlfs.dir/snapshot/hlfs_take_snapshot.c.o"
  "CMakeFiles/hlfs.dir/snapshot/hlfs_find_inode_by_name.c.o"
  "CMakeFiles/hlfs.dir/snapshot/hlfs_rm_snapshot.c.o"
  "CMakeFiles/hlfs.dir/cache/cache_destroy.c.o"
  "CMakeFiles/hlfs.dir/cache/cache_set_write_cb.c.o"
  "CMakeFiles/hlfs.dir/cache/cache_helper.c.o"
  "CMakeFiles/hlfs.dir/cache/cache_query.c.o"
  "CMakeFiles/hlfs.dir/cache/cache_init.c.o"
  "CMakeFiles/hlfs.dir/cache/cache_insert.c.o"
  "CMakeFiles/hlfs.dir/cache/flush_worker.c.o"
  "CMakeFiles/hlfs.dir/cache/cache_sync.c.o"
  "CMakeFiles/hlfs.dir/icache/icache_query.c.o"
  "CMakeFiles/hlfs.dir/icache/icache_exist.c.o"
  "CMakeFiles/hlfs.dir/icache/icache_init.c.o"
  "CMakeFiles/hlfs.dir/icache/icache_destroy.c.o"
  "CMakeFiles/hlfs.dir/icache/icache_insert.c.o"
  "CMakeFiles/hlfs.dir/clone/clone.c.o"
  "CMakeFiles/hlfs.dir/compress/minilzo.c.o"
  "CMakeFiles/hlfs.dir/compress/compress.c.o"
  "/home/edsionte/hlfs/output/lib32/libhlfs.pdb"
  "/home/edsionte/hlfs/output/lib32/libhlfs.so"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang C)
  INCLUDE(CMakeFiles/hlfs.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)