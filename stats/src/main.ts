interface PerStar {
	get_star_ts: string;
}

interface User {
	completion_day_level: {[key0: number]: { [key1: number]: PerStar }};
	last_star_ts: number;
	stars: number;
	local_score: number;
	global_score: number;
	id: number;
	name: string | null;
}

interface Leaderboard {
	owner_id: number;
	members: {[key: string]: User};
}

interface MyFmt {
	name: string;
	id: string;
	time1: number | null;
	time2: number | null;
}

window.addEventListener('load', function() {
	function removeAll(o: HTMLElement): void {
		while (o.firstChild)
			o.removeChild(o.firstChild);
	}
	function go(data: Leaderboard): void {
		let arr: MyFmt[] = [];
		for (let id in data.members) {
			let name = data.members[id].name;
			let item: MyFmt = { name: '', id: id, time1: null, time2: null };
			if (name != null)
				item.name = name;
			else
				item.name = '(#' + id.toString() + ')';
			arr.push(item);
		}
		(function() {
			let div = document.getElementById('top-links');
			if (! div)
				return;
			removeAll(div);
			for (let day = 1; day <= 25; ++day) {
				let btn = document.createElement('button');
				btn.append(document.createTextNode(day.toString()));
				btn.addEventListener('click', (e) => sort_day(day));
				div.append(btn);
			}
		})();
		function sort_day(day: number): void {
			(function() {
				let h1 = document.getElementById('top-label');
				if (! h1)
					return;
				removeAll(h1);
				h1.append(document.createTextNode('Day ' + day.toString()));
			})();
			let star1: MyFmt[] = [];
			let star2: MyFmt[] = [];
			for (let person of arr) {
				let today = data.members[person.id].completion_day_level[day];
				if (! today)
					continue;
				let t1 = today[1] ? today[1].get_star_ts : null;
				let t2 = today[2] ? today[2].get_star_ts : null;
				person.time1 = t1 ? parseInt(t1) : null;
				person.time2 = t2 ? parseInt(t2) : null;
				star1.push(person);
				star2.push(person);
			}
			function cmp1(a: MyFmt, b:MyFmt): number {
				if (a.time1 == null)
					return 1;
				if (b.time1 == null)
					return -1;
				return a.time1 - b.time1;
			}
			function cmp2(a: MyFmt, b:MyFmt): number {
				if (a.time2 == null)
					return 1;
				if (b.time2 == null)
					return -1;
				return a.time2 - b.time2;
			}
			star1.sort((a, b) => {
				if (a.time1 == b.time1)
					return cmp2(a, b);
				return cmp1(a, b);
			});
			star2.sort((a, b) => {
				if (a.time2 == b.time2)
					return cmp1(a, b);
				return cmp2(a, b);
			});
			function fmt_time_span(utcSecA: number | null, utcSecB : number | null): string {
				if (utcSecA == null || utcSecB == null)
					return '-';
				const diff = utcSecB - utcSecA;
				const days = Math.floor(diff / 86400);
				function fix(div: number, mod: number): string {
					let ret = (Math.floor(diff / div) % mod).toString();
					if (ret.length === 1)
						ret = '0' + ret;
					return ret;
				}
				let ret = fix(3600, 24) + ':' + fix(60, 60) + ':' + fix(1, 60);
				if (days != 0)
					ret += ' (' + days.toString() + ')';
				return ret;
			}
			function fmt_time(utcSecOrNull: number | null, dateOffset: number): string {
				if (utcSecOrNull == null)
					return 'DNF';
				// minus 5 hours since it takes place in EST time
				const fiveHours = 18000;
				let utcSec: number = utcSecOrNull - fiveHours;
				dateOffset = Math.floor((utcSec - dateOffset + fiveHours) / 86400);
				function fix(div: number, mod: number): string {
					let ret = (Math.floor(utcSec / div) % mod).toString();
					if (ret.length === 1)
						ret = '0' + ret;
					return ret;
				}
				let ret = fix(3600, 24) + ':' + fix(60, 60) + ':' + fix(1, 60);
				if (dateOffset != 0)
					ret += ' (' + dateOffset.toString() + ')';
				return ret;
			}
			function place_row(o: HTMLElement, person: MyFmt, rank: number, dateOffset: number): void {
				let td0 = document.createElement('td');
				let td1 = document.createElement('td');
				let td2 = document.createElement('td');
				let td3 = document.createElement('td');
				let td4 = document.createElement('td');
				td0.append(document.createTextNode(rank.toString()));
				td1.append(document.createTextNode(person.name));
				td2.append(document.createTextNode(fmt_time(person.time1, dateOffset)));
				td3.append(document.createTextNode(fmt_time(person.time2, dateOffset)));
				td4.append(document.createTextNode(fmt_time_span(person.time1, person.time2)));
				let tr = document.createElement('tr');
				tr.append(td0);
				tr.append(td1);
				tr.append(td2);
				tr.append(td3);
				tr.append(td4);
				o.append(tr);
			}
			function place_labels(o: HTMLElement, labels: string[]): void {
				let tr = document.createElement('tr');
				for (let label of labels) {
					let td = document.createElement('td');
					td.append(document.createTextNode(label));
					td.style.fontWeight = 'bold';
					tr.append(td);
				}
				o.append(tr);
			}
			(function() {
				let table1 = document.getElementById('table1');
				let table2 = document.getElementById('table2');
				if (! table1 || ! table2)
					return;
				let dateOffsetOrNull = (star1.length > 0) ? star1[0].time1 : 0;
				let dateOffset = 0;
				if (dateOffsetOrNull != null)
					dateOffset = dateOffsetOrNull;
				removeAll(table1);
				removeAll(table2);
				let labels: string[] = [ '', 'Name', 'Star 1', 'Star 2', 'Diff' ];
				if (star1.length > 0)
					place_labels(table1, labels);
				if (star2.length > 0)
					place_labels(table2, labels);
				for (let i = 0; i < star1.length; ++i) {
					place_row(table1, star1[i], i + 1, dateOffset);
				}
				for (let i = 0; i < star2.length; ++i)  {
					place_row(table2, star2[i], i + 1, dateOffset);
				}
			})();
		}
	}
	function load_file(): void {
		let req = new XMLHttpRequest();
		req.addEventListener('load', (e) => {
			let data: Leaderboard = JSON.parse(req.responseText);
			go(data);
		});
		req.addEventListener('timeout', (e) => {
			console.log('timeout');
		});
		req.addEventListener('error', (e) => {
			console.log('error!');
		});
		req.timeout = 5000;
		req.overrideMimeType('text/plain');
		req.open('GET', './data.json?t=' + Date.now().toString());
		req.send();
	}
	load_file();
	(function() {
		let btn = document.getElementById('reload-btn');
		if (! btn)
			return;
		btn.addEventListener('click', (e) => load_file());
	})();
});
