#!/usr/bin/env ruby

def to_view(sqlfile, dstfile, viewname, stdin_alt)
  File.open(sqlfile, "r") do |fi|
    File.open(dstfile, "a") do |fo|
      l = fi.readline until l =~ /^WITH/
      fo.puts "CREATE VIEW #{viewname} AS"
      fo.puts l
      fi.each_line do |l|
        case l
        when /SELECT i FROM stdin/
          fo.puts "  (#{stdin_alt}) stdin,"
        when /^SELECT writefile/
          fo.puts "SELECT stdout FROM elvm WHERE running = 0;"
        else
          fo.puts l
        end
      end
    end
  end
end

mksql,ccsql,elcsql = ARGV

File.open(mksql, "w") do |fo|
  fo.puts <<_SQL_
CREATE TABLE src(b BLOB);
CREATE TABLE eir(b BLOB);
CREATE TABLE option(target TEXT);
INSERT INTO option(target) VALUES('rb');
CREATE TABLE supported_targets(option_target, language);
INSERT INTO supported_targets(option_target, language)
VALUES
_SQL_
  File.open("supported_targets.txt", "r") do |fi|
    fo.puts "('" + fi.read.chomp.split(/\n/).join("'),\n('").split(/\t/).join("','") + "');"
  end
end

to_view(ccsql, mksql, "elvm_8cc", "SELECT b FROM src")
to_view(elcsql, mksql, "elvm_elc",
  "SELECT (SELECT target FROM option) || char(10) || b FROM eir")
to_view(elcsql, mksql, "elvm_8cc_elc",
  "SELECT (SELECT target FROM option) || char(10) || stdout FROM elvm_8cc")
