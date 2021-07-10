// pages/category/category.js

Page({
  /**
   * 页面的初始数据
   */
  data: {
    
    toview:'nav-0',
    active:0,
     bb:[
      {
        parkingname:"现代光谷世贸中心停车场",
        dis:"833",
        charging: "30分钟内免费，首价10元/h",
        num: "68",
        sta: "剩余"
      },
      {
        parkingname:"光谷智慧园停车场",
        dis:"524",
        charging: "30分钟内免费，首价10元/h",
        num: "45",
        sta: "剩余"
      },
      {
        parkingname:"光谷金融港停车场",
        dis:"708",
        charging: "30分钟内免费，首价10元/h",
        num: "0",
        sta: "已满"
      },
      {
        parkingname:"光谷汇金中心停车场",
        dis:"319",
        charging: "30分钟内免费，首价10元/h",
        num: "25",
        sta: "剩余"
      },
      {
        parkingname:"中国太平停车场",
        dis:"960",
        charging: "30分钟内免费，首价10元/h",
        num: "88",
        sta: "剩余"
      },   
      {
        parkingname:"青年公寓停车场",
        dis:"280",
        charging: "30分钟内免费，首价10元/h",
        num: "36",
        sta: "剩余"
      },  
      {
        parkingname:"城市之光停车场",
        dis:"985",
        charging: "30分钟内免费，首价10元/h",
        num: "18",
        sta: "剩余"
      },   
      {
        parkingname:"当代国际花园停车场",
        dis:"1024",
        charging: "30分钟内免费，首价10元/h",
        num: "58",
        sta: "剩余"
      }, 
      {
        parkingname:"泰康人寿大厦停车场",
        dis:"789",
        charging: "30分钟内免费，首价10元/h",
        num: "66",
        sta: "剩余"
      }, 
      {
        parkingname:"创客星停车场",
        dis:"656",
        charging: "30分钟内免费，首价10元/h",
        num: "72",
        sta: "剩余"
      }, 
     ]
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    this.setData({
      
    })
  },
 

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {
  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {

  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {

  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {

  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {

  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {

  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {

  }
})