# coding=utf-8
#!/usr/bin/env python

import re
import urllib2, cookielib, urllib
from BeautifulSoup  import BeautifulSoup

#get __VIEWSTATE's value (because it work with asp)
def get_viewstate(req):
    stream = urllib2.urlopen(req)
    soup = BeautifulSoup(stream)
    return soup.findAll(attrs = {'name': re.compile("__VIEWSTATE")})[0]['value']

#use cookie
opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cookielib.CookieJar()))
urllib2.install_opener(opener)

#cookie = "ASP.NET_SessionId=yntax145x2gfgkytzhx4vw45"
username = raw_input('学号:')
password = raw_input('密码:')
log_url = 'http://202.120.108.62/ecustedu/StudentLogin.aspx'
req = urllib2.Request(log_url)
log_viewstate = get_viewstate(req)

postdata = urllib.urlencode({'__VIEWSTATE': log_viewstate, 'TxtStudentId': username, 'TxtPassword': password, 'BtnLogin': '登录'})

login_response = urllib2.urlopen(req, postdata)

def print_table(stream):
    #打印课表
    soup = BeautifulSoup(stream)
    for tr in soup.findAll(name = re.compile("tr")):
        if tr.td.tr:
            continue
        else:
            for td in tr.findAll(name = re.compile("td")):
                print td.text, ' ' * (20 - 2 * len(td.text)),
        print ''

url_now = 'http://202.120.108.62/ecustedu/E_SelectCourse/ScInFormation/syllabus.aspx'

req = urllib2.Request(url_now)
#req.add_header('Cookie', cookie)
viewstate = get_viewstate(req)

post_now = urllib.urlencode({"__VIEWSTATE": viewstate, "selyeartermflag": "这学期", "bttn_search": "查询"})
list_response = urllib2.urlopen(req, post_now)

print_table(list_response)

