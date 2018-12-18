Page({
  data: {
    device_id: '503083986',
    api_key: 'Lz6kPLRd=Qe9Qt=JkoUPnkyLLys=',
    a: '2',
    motto: "         "
  },
  validate: function () {
    wx.navigateTo({
      url: '../xingxi/xingxi',
    })
  },

    send1: function (a) {
    var _this = this;
    wx.request({
      url: 'http://api.heclouds.com/devices/503083986/datapoints?type=3',
      header: {
        "Content-Type": "application/json",
        "api-key": "Lz6kPLRd=Qe9Qt=JkoUPnkyLLys="
        // "Host": "api.heclouds.com"
      },
      method: 'post',
      data: {
        "a": "2"
      },
      success: function (res) {
        console.log(res)
      },
      fail: function (res) {
        console.log(res)
      }
    })
      setTimeout(function () {
        wx.request({
          url: 'http://api.heclouds.com/devices/503083986/datapoints?type=3',
          header: {
            "Content-Type": "application/json",
            "api-key": "Lz6kPLRd=Qe9Qt=JkoUPnkyLLys="
            // "Host": "api.heclouds.com"
          },
          method: 'post',
          data: {
            "a": "1"
          },
          success: function (res) {
            console.log(res)
          },
          fail: function (res) {
            console.log(res)
          }
        })
      }, 20000) //延迟时间 这里是8秒
      

    },
 
  getDataFromOneNet: function () {
    //从oneNET请求我们的Wi-Fi气象站的数据
    const requestTask = wx.request({
      url: 'http://api.heclouds.com/devices/503083986/datapoints?datastream_id=Light,Temperature,Humidity&limit=15',
      header: {
        'content-type': 'application/json',
        'api-key': 'Lz6kPLRd=Qe9Qt=JkoUPnkyLLys='
      },
      success: function (res) {
        //console.log(res.data)
        //拿到数据后保存到全局数据
        var app = getApp()
        app.globalData.temperature = res.data.data.datastreams[0]
        app.globalData.light = res.data.data.datastreams[1]
        app.globalData.humidity = res.data.data.datastreams[2]
        console.log(app.globalData.light)
        //跳转到天气页面，根据拿到的数据绘图
        wx.navigateTo({
          url: '../xingxi/xingxi',
        })
      },

      fail: function (res) {
        console.log("fail!!!")
      },

      complete: function (res) {
        console.log("end")
      }
    })
  },


})