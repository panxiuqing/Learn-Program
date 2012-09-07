#coding=utf-8
#!/usr/bin/env python

import os
import urllib2, json


#api_string = 'http://www.douban.com/j/app/radio/people?app_name=radio_desktop_win&version=100&user_id=&expire=&token=&sid=&h=&channel=1&type=n'

app_name = 'radio_desktop_win'
version = '100'
user_id = ''
expire = ''
token = ''
sid = ''
h = ''
channel = '1'
Type = 'n'

fm_url = 'http://www.douban.com/j/app/radio/people?'
get_list = [('app_name=', app_name),
            ('version=', version),
            ('user_id=', user_id),
            ('expire=', expire),
            ('token=', token),
            ('sid=', sid),
            ('h=', h),
            ('channel=', channel),
            ('type=', Type)]
get_string = "&".join(key + value for key, value in get_list)
final_url = fm_url + get_string

mp3_info = {'title': '曲目', 'picture': '专辑封面', 'albumtitle': '专辑', 'company': '公司', 'artist': '演唱者', 'like': '喜欢'}

while True:
    resp = urllib2.urlopen(final_url)
    play_list = json.loads(resp.read())['song']
    for mp3 in play_list:
        for key in mp3_info:
            print mp3_info[key], ':', (12 - 2*len(mp3_info[key]))*' ' , mp3[key]
        os.system('mplayer %s > /dev/null' % mp3['url'])
