
local http = require "http"
local io = require "io"
local shortport = require "shortport"
local stdnse = require "stdnse"


description = [[
Comprobamos si el host remoto tiene el puerto indicado activo, en ese caso, obtenemos el /index.html y lo almacenamos en un fichero "index.html"
]]

---
-- @usage
-- nmap -p 80 <ip> --script=http-index 
--
-- Version 0.1
-- Created 23/04/2017 - v0.1 - created by R&R_Asociados
--

author = "R&R Asociados"
license = "Open License"
categories = {"discovery"}

portrule=shortport.http

action = function( host, port )
local result
path = "/index.html"
local request_type
	result = http.get(host, port, path)
	request_type = "GET"


local f = io.open("index.html","w")
	io.output(f)
	io.write(table.tostring( result ))

	return
end

function table.val_to_str ( v )
  if "string" == type( v ) then
	string.gsub( v, "\n", "\\n" )
	if string.match( string.gsub(v,"[^'\"]",""), '^"+$' ) then
      return "'" .. v .. "'"
    end
    return '"' .. string.gsub(v,'"', '\\"' ) .. '"'
  else
    return "table" == type( v ) and table.tostring( v ) or
      tostring( v )
  end
end

function table.key_to_str ( k )
  if "string" == type( k ) and string.match( k, "^[_%a][_%a%d]*$" ) then
    return k
  else
    return "[" .. table.val_to_str( k ) .. "]"
  end
end

function table.tostring( tbl )
  local result, done = {}, {}
  for k, v in ipairs( tbl ) do
    table.insert( result, table.val_to_str( v ) )
    done[ k ] = true
  end
  for k, v in pairs( tbl ) do
    if not done[ k ] then
      table.insert( result,
        table.key_to_str( k ) .. "=" .. table.val_to_str( v ) )
    end
  end
  return "{" .. table.concat( result, "," ) .. "}"
end

