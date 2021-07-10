// pages/mycoupon/mycoupon.js
var app = getApp()
Page({

  /**
   * 页面的初始数据
   */
  data: {
    navbar: ['未使用', '已使用', '已过期'],
    currentTab: 0,
    coupons:[
    {
      quanname: "3元停车优惠券",
      manjian: "满10元可用",
      usetime: "有效期: 2021.05.01-2021.06.30",
      num: "3",     
    },
    {
      quanname: "5元停车优惠券",
      manjian: "满20元可用",
      usetime: "有效期: 2021.06.01-2021.07.30",
      num: "5", 
    },
    {
      quanname: "10元停车优惠券",
      manjian: "满30元可用",
      usetime: "有效期: 2021.07.01-2021.08.31",
      num: "10",
    },
    {
      quanname: "3元停车优惠券",
      manjian: "满10元可用",
      usetime: "有效期: 2021.05.01-2021.06.30",
      num: "3",
    },
    {
      quanname: "5元停车优惠券",
      manjian: "满20元可用",
      usetime: "有效期: 2021.06.01-2021.07.30",
      num: "5", 
    },
    {
      quanname: "10元停车优惠券",
      manjian: "满30元可用",
      usetime: "有效期: 2021.07.01-2021.08.31",
      num: "10",
    },
    ]
  },

  navbarTap: function(e){
    this.setData({
      currentTab: e.currentTarget.dataset.idx
    })
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {

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