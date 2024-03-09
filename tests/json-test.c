#include "ipc-bus/raw-raii.h"
#include "logger.h"
#include <bits/stdint-uintn.h>
#include <cJSON.h>
#include <ipc-bus/utils.h>
#include <mln_core.h>
#include <mln_json.h>
#include <mln_string.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t read_file(const char *fname, void *buf, uint32_t size) {
  declare_fd(fd) = open(fname, O_RDONLY);
  declare_charptr(_buf) = cast(char *, malloc(BUFSIZ));
  assert(fd > 0);
  uint32_t total_read_len = 0;
  while (true) {
    ssize_t read_len = read(fd, _buf, BUFSIZ);
    if (!read_len)
      break;
    if ((total_read_len + read_len) < size) {
      memcpy(buf + total_read_len, _buf, read_len);
      total_read_len += read_len;
    }
  }
  return total_read_len;
}
static cJSON *find_json_node(cJSON *root, const char *e_name) {
  cJSON *node_out = NULL;
  if (cJSON_IsObject(root)) {
    cJSON *node = cJSON_GetObjectItemCaseSensitive(root, e_name);
    if (node) {
      node_out = node;
      logger_debug("found: %s, type=%d\n", e_name, node->type);
      return node_out;
    }
  }
  return node_out;
}

#define JSON_FILE_FNAME "/tmp/0.json"
#define JSON_FILE_FNAME1 "/tmp/miner/leeyg/linkcube-1qt4aiv9.json"
int main(int argc, char *argv[]) {
  declare_charptr(json_content) = cast(char *, malloc(BUFSIZ));
  logger_init(NULL, LOGGER_LEVEL_DEBUG);

  uint32_t file_len = read_file(JSON_FILE_FNAME, json_content, BUFSIZ);
  json_content[file_len] = '\0';
  logger_debug("file=%s, len=%d\n", JSON_FILE_FNAME, file_len);
  logger_debug("json dump\n%s\n----\n", json_content);

  cJSON *json;
  json = cJSON_Parse(json_content);
  logger_warn("json_ptr=%p,type=%d, %d\n", json, json->type, cJSON_Object);
  ut_assert(json != NULL);
  cJSON *tasks_node = find_json_node(json, "tasks");
  cJSON *eth_node = find_json_node(tasks_node, "eth");
  ut_assert(eth_node != NULL);
  logger_debug("eth_node.type = %d\n", cJSON_IsObject(eth_node));
  logger_debug("eth_node.type = %d\n", cJSON_IsArray(eth_node));
  char *out = cJSON_Print(eth_node);
  logger_debug("%s\n", out);
  return 0;
}