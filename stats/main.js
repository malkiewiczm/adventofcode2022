"use strict";
window.addEventListener('load', function () {
    function removeAll(o) {
        while (o.firstChild)
            o.removeChild(o.firstChild);
    }
    function go(data) {
        var arr = [];
        for (var id in data.members) {
            var name_1 = data.members[id].name;
            var item = { name: '', id: id, time1: null, time2: null };
            if (name_1 != null)
                item.name = name_1;
            else
                item.name = '(#' + id.toString() + ')';
            arr.push(item);
        }
        (function () {
            var div = document.getElementById('top-links');
            if (!div)
                return;
            removeAll(div);
            var _loop_1 = function (day) {
                var btn = document.createElement('button');
                btn.append(document.createTextNode(day.toString()));
                btn.addEventListener('click', function (e) { return sort_day(day); });
                div.append(btn);
            };
            for (var day = 1; day <= 25; ++day) {
                _loop_1(day);
            }
        })();
        function sort_day(day) {
            (function () {
                var h1 = document.getElementById('top-label');
                if (!h1)
                    return;
                removeAll(h1);
                h1.append(document.createTextNode('Day ' + day.toString()));
            })();
            var star1 = [];
            var star2 = [];
            for (var _i = 0, arr_1 = arr; _i < arr_1.length; _i++) {
                var person = arr_1[_i];
                var today = data.members[person.id].completion_day_level[day];
                if (!today)
                    continue;
                var t1 = today[1] ? today[1].get_star_ts : null;
                var t2 = today[2] ? today[2].get_star_ts : null;
                person.time1 = t1 ? parseInt(t1) : null;
                person.time2 = t2 ? parseInt(t2) : null;
                star1.push(person);
                star2.push(person);
            }
            function cmp1(a, b) {
                if (a.time1 == null)
                    return 1;
                if (b.time1 == null)
                    return -1;
                return a.time1 - b.time1;
            }
            function cmp2(a, b) {
                if (a.time2 == null)
                    return 1;
                if (b.time2 == null)
                    return -1;
                return a.time2 - b.time2;
            }
            star1.sort(function (a, b) {
                if (a.time1 == b.time1)
                    return cmp2(a, b);
                return cmp1(a, b);
            });
            star2.sort(function (a, b) {
                if (a.time2 == b.time2)
                    return cmp1(a, b);
                return cmp2(a, b);
            });
            function fmt_time_span(utcSecA, utcSecB) {
                if (utcSecA == null || utcSecB == null)
                    return '-';
                var diff = utcSecB - utcSecA;
                var days = Math.floor(diff / 86400);
                function fix(div, mod) {
                    var ret = (Math.floor(diff / div) % mod).toString();
                    if (ret.length === 1)
                        ret = '0' + ret;
                    return ret;
                }
                var ret = fix(3600, 24) + ':' + fix(60, 60) + ':' + fix(1, 60);
                if (days != 0)
                    ret += ' (' + days.toString() + ')';
                return ret;
            }
            function fmt_time(utcSecOrNull, dateOffset) {
                if (utcSecOrNull == null)
                    return 'DNF';
                var fiveHours = 18000;
                var utcSec = utcSecOrNull - fiveHours;
                dateOffset = Math.floor((utcSec - dateOffset + fiveHours) / 86400);
                function fix(div, mod) {
                    var ret = (Math.floor(utcSec / div) % mod).toString();
                    if (ret.length === 1)
                        ret = '0' + ret;
                    return ret;
                }
                var ret = fix(3600, 24) + ':' + fix(60, 60) + ':' + fix(1, 60);
                if (dateOffset != 0)
                    ret += ' (' + dateOffset.toString() + ')';
                return ret;
            }
            function place_row(o, person, rank, dateOffset) {
                var td0 = document.createElement('td');
                var td1 = document.createElement('td');
                var td2 = document.createElement('td');
                var td3 = document.createElement('td');
                var td4 = document.createElement('td');
                td0.append(document.createTextNode(rank.toString()));
                td1.append(document.createTextNode(person.name));
                td2.append(document.createTextNode(fmt_time(person.time1, dateOffset)));
                td3.append(document.createTextNode(fmt_time(person.time2, dateOffset)));
                td4.append(document.createTextNode(fmt_time_span(person.time1, person.time2)));
                var tr = document.createElement('tr');
                tr.append(td0);
                tr.append(td1);
                tr.append(td2);
                tr.append(td3);
                tr.append(td4);
                o.append(tr);
            }
            function place_labels(o, labels) {
                var tr = document.createElement('tr');
                for (var _i = 0, labels_1 = labels; _i < labels_1.length; _i++) {
                    var label = labels_1[_i];
                    var td = document.createElement('td');
                    td.append(document.createTextNode(label));
                    td.style.fontWeight = 'bold';
                    tr.append(td);
                }
                o.append(tr);
            }
            (function () {
                var table1 = document.getElementById('table1');
                var table2 = document.getElementById('table2');
                if (!table1 || !table2)
                    return;
                var dateOffsetOrNull = (star1.length > 0) ? star1[0].time1 : 0;
                var dateOffset = 0;
                if (dateOffsetOrNull != null)
                    dateOffset = dateOffsetOrNull;
                removeAll(table1);
                removeAll(table2);
                var labels = ['', 'Name', 'Star 1', 'Star 2', 'Diff'];
                if (star1.length > 0)
                    place_labels(table1, labels);
                if (star2.length > 0)
                    place_labels(table2, labels);
                for (var i = 0; i < star1.length; ++i) {
                    place_row(table1, star1[i], i + 1, dateOffset);
                }
                for (var i = 0; i < star2.length; ++i) {
                    place_row(table2, star2[i], i + 1, dateOffset);
                }
            })();
        }
    }
    function load_file() {
        var req = new XMLHttpRequest();
        req.addEventListener('load', function (e) {
            var data = JSON.parse(req.responseText);
            go(data);
        });
        req.addEventListener('timeout', function (e) {
            console.log('timeout');
        });
        req.addEventListener('error', function (e) {
            console.log('error!');
        });
        req.timeout = 5000;
        req.overrideMimeType('text/plain');
        req.open('GET', './data.json?t=' + Date.now().toString());
        req.send();
    }
    load_file();
    (function () {
        var btn = document.getElementById('reload-btn');
        if (!btn)
            return;
        btn.addEventListener('click', function (e) { return load_file(); });
    })();
});
