# Generated by Django 2.0.7 on 2018-09-02 01:57

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('infinityroom', '0008_auto_20180819_2151'),
    ]

    operations = [
        migrations.AddField(
            model_name='device',
            name='devGrp_db',
            field=models.TextField(default=None),
            preserve_default=False,
        ),
    ]