#!/usr/bin/env lua54

local json=require("cjson")
local socket=require("socket")
local redis = require 'redis'


local BUS_ID_KEY = "bus_id"
local jsonprc = {
    method = "ping",
    parms = { ping="ping", url="/tmp", do_fork=true },
    id=1
}
local function get_redis_instance()
  local r = redis.connect('192.168.1.10', 6379)
  return r
end
local r = get_redis_instance();
jsonprc.id = r:incr(BUS_ID_KEY)
local request = json.encode(jsonprc)

local host="localhost"
local unix_socket_path = "/tmp/bus_ipc"

local client = socket.tcp()
client:connect(host, 8001)
client:send(request)
print(request)
client:close()
