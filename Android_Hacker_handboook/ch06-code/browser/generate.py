#!/bin/python
#
# BrowserFuzz
#
# Amir Etemadieh
# Joshua J. Drake
#
# This file contains the logic for generating HTML data based on the 
# HTML tags in tags.py.
#

import random


TYPEDARRAY_TYPES = [
    'Array',
    'ArrayBuffer',
    'ArrayBufferView',
    'DataView',
    'StringView',
    'Int8Array',
    'Uint8Array',
    'Uint8ClampedArray',
    'Int16Array',
    'Uint16Array',
    'Int32Array',
    'Uint32Array',
    'Float32Array',
    'Float64Array'
]

#
# Generate a fuzz HTML page
#
def generate_page():
    page = """<html>
<head>
  <title>BrowserFuzz</title>
<script type='text/javascript'>
function boom() {
  console.log("========================= boom start =========================");

"""
    # first, create an array using a random type and size
    page += "  try { " + generate_var() + " } catch(e) { console.log(e); }\n"
    page += "\n"

    # now, make an assignment to a random type
    page += "  try { " + generate_assignment() + " } catch(e) { console.log(e); }\n"
    
    # finish the page
    page += """
  console.log("========================= boom end =========================");
}
</script>
</head>
<body onload='javascript:boom()'>
hi!
</body>
</html>
"""
    return page

def generate_var():
    vtype = random.choice(TYPEDARRAY_TYPES)
    vlen = rand_num()
    return "var arr1 = new %s(%d);" % (vtype, vlen)

def generate_assignment():
    vtype = random.choice(TYPEDARRAY_TYPES)
    return "var arr2 = new %s(arr1);" % (vtype)

#
# Pick a number, any number...
#
def rand_num():
    divisor = random.randrange(0x8) + 1
    dividend = (0x100000000 / divisor)
    if random.randrange(3) == 0:
        addend = random.randrange(10)
        addend -= 5
        dividend += addend
    return dividend

#
# for testing...
#   
if __name__ == "__main__":
    print generate_page(),

