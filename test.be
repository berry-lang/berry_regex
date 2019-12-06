import regex as re

def scanfile(fileName, pattern)
    print('match file <' .. fileName .. '>, pattern: ' .. pattern)
    f = open(fileName)
    for (i : re.finditer(pattern, f.read()))
        print('  match: ' .. i)
    end
end

scanfile('test.be', '\\w+')
