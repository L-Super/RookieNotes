import requests


def func1():
    s = requests.Session()
    # 第一步：发送一个请求，用于设置请求中的cookies
    # tips: http://httpbin.org能够用于测试http请求和响应
    s.get('http://httpbin.org/cookies/set/sessioncookie/123456789')
    # 第二步：再发送一个请求，用于查看当前请求中的cookies
    r = s.get("http://httpbin.org/cookies")
    print(r.text)


def func2():
    s = requests.Session()
    s.auth = ('user', 'pass')
    s.headers.update({'x-test': 'true'})

    print(s.headers)
    # both 'x-test' and 'x-test2' are sent
    r = s.get('https://httpbin.org/headers', headers={'x-test2': 'true'})
    print(r.request.headers)


if __name__ == '__main__':
    func1()
    func1()

    # 获取cookie
    cookie_dict = requests.utils.dict_from_cookiejar(s.cookies)
    print(cookie_dict)
