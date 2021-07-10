// pages/mycar/mycar.js
Page({

  /**
   * 页面的初始数据
   */
  data: {
    index:'请选择',
    array: ['大型汽车', '小型汽车', '新能源汽车', '专用汽车', '特种车', '电瓶车', '摩托车',],
    
  },

  gotoaddcar: function () {
    wx.navigateTo({
      url: '/pages/addcar/addcar',
    })
  },

  // 完成绑定车牌号码
  addCarNumber() {
    
  },
  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {

  },

  bindCarChange: function(e) {
    console.log('picker发送选择改变，携带值为', e.detail.value)
    this.setData({
      index: e.detail.value
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