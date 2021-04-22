// pages/shouye/shouye.js
var app = getApp();
var total1= new Array();
var total2= new Array();
var i=0;
var j=0;
var number1=0;
var number2=0;

Page({
  /*** 页面的初始数据 */
  data: {
    isLogin:'',
    device_ID:'',
    online:'',
    sound:'',
    LED:'',
    I:0,
    T:0,
    H:0,
    number1:0,
    number2:0,

    switch1Checked: false,
    switch2Checked: false,
    switch3Checked: false,
    title1:'登录成功',
    title2:'自动刷新成功',
    title3:'正在刷新',
  },

  /**
   * 获取数据函数
   * 参数t为打印数据
   * 可以获取是否在线以及其他传感器数据
   **/
  getData:function(t){
    var that = this;
    wx.request({
      url: 'http://api.heclouds.com/devices/' + app.globalData.g_phone,
      header: {
        'api-key': app.globalData.g_password
      },
      method: 'get',
      success: function (res) {
        if(res.data.errno==0){
          console.log(res),
          app.globalData.g_isLogin = true,
          app.globalData.g_online = res.data.data.online,
          that.setData({ 
            isLogin: true ,
            online : res.data.data.online
          })
          wx.showToast({
            title: t,
            icon: 'loading',
            duration: 1000
          })
        }
      },
      fail(res) {
        console.log("请求失败", res)
        wx.showToast({
          title: '设备ID或API-key错误，请联系经销商',
          icon: 'none',
          duration: 1500
        })
      }
    })
    wx.request({
      url: 'http://api.heclouds.com/devices/'+ app.globalData.g_phone+'/datastreams',
      header: {
        'api-key': app.globalData.g_password
      },
      method:"GET",
      success: function (res) {
        console.log(res.data)
        that.setData({ 
          T: res.data.data[0].current_value,
          H: res.data.data[1].current_value,
          I: res.data.data[2].current_value,
          
          //number1: res.data.data[4].current_value,
          //number2: res.data.data[3].current_value,
        })
       /*  total1[i]=res.data.data[3].current_value,
         total2[j]=res.data.data[4].current_value,
        i++,
        j++,
        number1=total1.length,
        number2=total2.length,
        console.log(number1)
        console.log(total1)
        console.log(number2)
        console.log(total2)
        that.setData({
          number1:total1.length,
          number2:total2.length
   });*/
      },
      fail(res) {
        console.log("请求失败",res)
        // deviceConnected = false
      }
    })
  },

  /*** 生命周期函数--监听页面加载 */
  onLoad: function (options) {
    var that = this
    that.setData({
      isLogin: app.globalData.g_isLogin,
      device_ID:app.globalData.g_phone,
    });
    wx.getStorage({
      key: 'phoneStorage',
      success: function(res) {
        app.globalData.g_phone = res.data
        //that.data.device_ID = res.data
        console.log("获取缓存成功",res.data)
      },
      fail: function(res) {console.log("获取缓存失败")},
    })
    wx.getStorage({
      key: 'passwordStorage',
      success: function(res) {
        app.globalData.g_password = res.data
        console.log("获取缓存成功",res.data)
      },
      fail: function(res) {console.log("获取缓存失败")},
    })
    
    setInterval(function () {
      if (that.data.online == true && app.globalData.g_phone.length>0){   //判断设备在线再请求更新数据
        that.getData(that.data);
        //console.log("轮播请求1秒触发一次");
      }else{
        console.log("设备不在线或者其他错误")
      }
    }, 5000)    //代表1秒钟发送一次请求
  },



  /*** 生命周期函数--监听页面初次渲染完成 */
  onReady: function () {
    var that = this;
    /*** 加入了自动刷新功能，无需一下代码更新数据 */
    if (that.data.isLogin = true && that.data.device_ID.length>0){
      that.getData(that.data.title1);
    }
    else{
      console.log("密码为空")
      console.log(that.data.isLogin,app.globalData.g_phone.length)
    }
    console.log("OnReady在运行")
  },

  /*** 生命周期函数--监听页面显示 */
  onShow: function () {
    var that = this
    that.setData({
      isLogin: app.globalData.g_isLogin,
      device_ID : wx.getStorageSync('phoneStorage'),
      online : app.globalData.g_online,
    });
    console.log("onshow在运行")
  },

 total: function() {
    var that = this
if(that.Data.sound="1"){
number1: that.data.number1+ 1
}
},

//fever: function() {
 // var that = this
//if(that.Data.LED='1'){
//number2: that.data.number2 + 1}
//},
  cancel:function(e){
    this.setData({
      T:0,
      H:0,
      I:0,
      number1:0,
      number2:0,

      total:{}
    })
  },

/*
  onLoad: function () {
    // 调用函数时，传入new Date()参数，返回值是日期和时间
    var time = util.formatTime(new Date());
    // 再通过setData更改Page()里面的data，动态更新页面的数据
    this.setData({
      time: time
    });
  },

  onLoad: function () {
    var DATE = util.formatDate(new Date());
    this.setData({
    date: DATE,   
    });
  },
*/
/*  recordtotal: function() {
    var that = this;
  var arr1 = new Array()
  if(time="24:00"){
  arr1: that.data.number1}
  },
  
  recordfever: function() {
    var that = this;
  var arr2 = new Array()
  if(time="24:00"){
  arr2: that.data.number2}
  },
  
  recorddate: function() {
    var that = this;
  var arr3 = new Array()
  {
  arr3: that.data.Date}
  }, */

  /** 生命周期函数--监听页面隐藏 */
  onHide: function () {

  },

  /*** 生命周期函数--监听页面卸载 */
  onUnload: function () {

  },

  /*** 页面相关事件处理函数--监听用户下拉动作 */
  onPullDownRefresh: function () {
    var that = this;
    that.getData(that.data.title3);
    wx.stopPullDownRefresh();
  },

  /*** 页面上拉触底事件的处理函数 */
  onReachBottom: function () {

  },

  /*** 用户点击右上角分享 */
  onShareAppMessage: function () {

  }
})