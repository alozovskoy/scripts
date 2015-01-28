#!/usr/bin/env python

import os
import subprocess

d_dict = {}
w_dict = {}
menu_items = ''

prompt = os.getenv('USER') + '@' + os.uname()[1]

w_raw, err = subprocess.Popen(
    'wmctrl -l',
    shell=True,
    stdout=subprocess.PIPE).communicate()
w_raw = w_raw.split('\n')
w_raw.remove('')

d_raw, err = subprocess.Popen(
    'wmctrl -d',
    shell=True,
    stdout=subprocess.PIPE).communicate()
d_raw = d_raw.split('\n')
d_raw.remove('')

for item in d_raw:
    item.split()
    d_dict.update({item[0]: item[-1]})

for item in w_raw:
    w_dict[item.split()[0]] = {}
    w_dict[item.split()[0]].update({'desk': item.split()[1]})
    w_dict[item.split()[0]].update({'name': item.split()[3:]})

for index, item in enumerate(w_dict):
    menu_items += \
        d_dict[w_dict[item]['desk']] + ' - ' + ' '.join(w_dict[item]['name'])
    if index != (len(w_dict) - 1):
        menu_items += '\n'

window, err = subprocess.Popen('echo \'' + menu_items + '\'| dmenu -i -p "' +
                               prompt +
                               '" -l ' +
                               str(len(w_dict)) +
                               ' -fn \*',
                               shell=True,
                               stdout=subprocess.PIPE).communicate()

if window:
    d_dict = dict(zip(d_dict.values(), d_dict.keys()))
    t_desk = d_dict[window.split(' - ')[0]]
    for item in w_dict:
        if ' '.join(w_dict[item]['name']) == \
                ' '.join(window.split(' - ')[1:]).strip():
            t_win = item

    subprocess.Popen('echo ' + t_desk + ' | wmctrl -i -a ' + t_win, shell=True)
