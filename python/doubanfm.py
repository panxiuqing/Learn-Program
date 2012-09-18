#coding=utf-8
#!/usr/bin/env python

import subprocess
import urllib2, urllib, json
import tempfile
import mplayer


#api_string = 'http://www.douban.com/j/app/radio/people?app_name=radio_desktop_win&version=100&user_id=&expire=&token=&sid=&h=&channel=1&type=n'

app_name = 'radio_desktop_win'
version = '100'
user_id = ''
expire = ''
token = ''
sid = ''
h = ''
channel = '7'
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

img_file = tempfile.NamedTemporaryFile()

p = mplayer.Player()

while True:
    resp = urllib2.urlopen(final_url)
    play_list = json.loads(resp.read())['song']
    for mp3 in play_list:
        mp3_img_url = mp3['picture']
        urllib.urlretrieve(mp3_img_url, img_file.name)
        mp3_title = mp3['title'].encode('utf-8')
        mp3_info = (mp3['albumtitle'] + '\n' + mp3['artist'])
        subprocess.call(['notify-send', '-i', img_file.name, mp3_title, mp3_info])
        p.loadfile(mp3['url'])
